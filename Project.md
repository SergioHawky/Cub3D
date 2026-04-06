# cub3D — Documentação do Projeto

> Documentação técnica detalhada do projeto cub3D, desenvolvido em C com a biblioteca MiniLibX.  
> Escrito para referência futura: explica **o quê**, **como** e **porquê** de cada decisão.

---

## Índice

1. [Visão Geral](#visão-geral)
2. [Estrutura de Dados](#estrutura-de-dados)
3. [Macros e Constantes](#macros-e-constantes)
4. [Fluxo Principal — `main.c`](#fluxo-principal--mainc)
5. [Parsing — `parsing.c`](#parsing--parsingc)
6. [Validação do Mapa — `map_checker_1.c`](#validação-do-mapa--map_checker_1c)
7. [Flood Fill — `map_checker_2.c`](#flood-fill--map_checker_2c)
8. [Eventos e Loop de Jogo — `event.c`](#eventos-e-loop-de-jogo--eventc)
9. [Input de Teclado — `key_handler.c`](#input-de-teclado--key_handlerc)
10. [Minimapa 2D — `minimap.c`](#minimapa-2d--minimapc)
11. [Jogador — `player.c`](#jogador--playerc)
12. [Raycasting — `raycasting.c`](#raycasting--raycastingc)
13. [Raios no Minimapa — `draw_ray_minimap()`](#raios-no-minimapa--draw_ray_minimap)
14. [Utilitários — `utils.c`](#utilitários--utilsc)

---

## Visão Geral

O cub3D é um raycaster em C inspirado no Wolfenstein 3D. O jogador navega num mapa 2D representado por uma grelha de caracteres, e o engine projeta uma perspetiva 3D em tempo real usando a técnica de **DDA (Digital Differential Analysis)**.

O projeto divide-se em duas grandes fases:
- **Parsing & Validação**: leitura do ficheiro `.cub`, verificação de caracteres, bordas e posição do jogador.
- **Engine de Renderização**: raycasting, projeção, movimento e eventos.

---

## Estrutura de Dados

### `t_img` — Imagem MiniLibX

```c
typedef struct s_img
{
    void    *img_ptr;   // ponteiro para a imagem MLX
    char    *addr;      // endereço de memória dos pixels
    int     bpp;        // bits por pixel (normalmente 32)
    int     endian;     // ordem dos bytes (big/little endian)
    int     line_len;   // tamanho em bytes de uma linha de pixels
} t_img;
```

A MiniLibX não expõe os pixels diretamente. Para escrever pixels eficientemente (sem chamar `mlx_pixel_put` por cada pixel, que é lento), pedimos o endereço de memória da imagem com `mlx_get_data_addr`. A fórmula para aceder ao pixel `(x, y)` é:

```
addr + (y * line_len + x * (bpp / 8))
```

Escrever diretamente neste endereço é ordens de magnitude mais rápido do que `mlx_pixel_put`.

---

### `t_map` — Mapa do Jogo

```c
typedef struct s_map
{
    char  **grid;               // grelha do mapa (array de strings)
    int     width;              // largura máxima do mapa
    int     height;             // número de linhas
    float   player_initial_x;  // coluna inicial do jogador
    float   player_initial_y;  // linha inicial do jogador
    char    player_initial_dir; // direção inicial: 'N', 'S', 'E', 'W'
    int     exit_x;            // coluna da saída ('A')
    int     exit_y;            // linha da saída ('A')
} t_map;
```

O mapa é armazenado como um array de strings (`char **grid`). Cada linha do ficheiro `.cub` torna-se um elemento do array. As linhas **não são padded** para o mesmo comprimento — têm tamanhos diferentes — por isso funções como `flood_fill` usam `ft_strlen(map->grid[y])` para saber o comprimento real de cada linha, em vez de usar `map->width`.

`player_initial_x` e `player_initial_y` são `float` porque são depois usados diretamente na posição do jogador (que é em vírgula flutuante).

---

### `t_player` — Estado do Jogador

```c
typedef struct s_player
{
    float x;        // posição X no mapa (em tiles, com decimais)
    float y;        // posição Y no mapa
    float dir_x;    // vetor direção X (normalizado)
    float dir_y;    // vetor direção Y
    float plane_x;  // vetor do plano da câmara X
    float plane_y;  // vetor do plano da câmara Y
    float move_speed;
    float rot_speed;
} t_player;
```

O raycaster usa dois vetores fundamentais:
- **`dir`**: o vetor que aponta para onde o jogador olha.
- **`plane`**: o vetor perpendicular ao `dir`, que representa o plano da câmara (o ecrã).

Estes dois vetores, em conjunto, determinam todos os raios lançados. Ver secção do raycasting para detalhes.

---

### `t_ray` — Estado de um Raio

```c
typedef struct s_ray
{
    float   ray_dir_x;    // direção do raio
    float   ray_dir_y;
    int     map_x;        // tile atual do raio
    int     map_y;
    float   delta_dist_x; // distância entre cruzamentos de grid em X
    float   delta_dist_y;
    float   side_dist_x;  // distância até ao próximo cruzamento X
    float   side_dist_y;
    int     step_x;       // direção de avanço: -1 ou +1
    int     step_y;
    int     side;         // 0 = parede N/S, 1 = parede E/W
    float   perp_dist;    // distância perpendicular (corrige fisheye)
} t_ray;
```

Cada coluna de pixels do ecrã tem um raio associado. A struct `t_ray` guarda o estado intermédio do algoritmo DDA para esse raio. Será explicada em detalhe na secção do raycasting.

---

### `t_key` e `t_data` — Input e Estado Global

```c
typedef enum e_key
{
    KEY_W = 0,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_COUNT
} t_key;

typedef struct s_data
{
    void      *mlx;
    void      *win;
    t_map     *map;
    t_player   player;
    t_img      img;
    int        keys[KEY_COUNT];
} t_data;
```

`keys[KEY_COUNT]` é um array de booleanos que regista quais as teclas atualmente premidas. Isto permite movimento suave: em vez de reagir a um evento de keypress de cada vez, o loop de jogo verifica este array a cada frame e move o jogador enquanto a tecla estiver premida.

---

## Macros e Constantes

```c
#define PX         32             // tamanho de cada tile em pixels (para o minimapa 2D)
#define WIDTH      1024           // largura da janela
#define HEIGHT     512            // altura da janela
#define SPEED      0.08f          // velocidade de movimento (em tiles/frame)
#define ROT_SPEED  0.20f          // velocidade de rotação (em radianos/frame)
#define PI         3.14159...     // PI para conversões de ângulo
#define FOV        (60.0f * PI / 180.0f)  // campo de visão em radianos
```

### Porquê `FOV = 60°`?

O FOV (Field of View) define a abertura angular do que o jogador vê. 60° é um valor clássico em raycasters:
- Muito pequeno (< 45°) → sensação de zoom/túnel
- Muito grande (> 90°) → distorção excessiva nas bordas
- 60–75° → proporcional e confortável para jogos em primeira pessoa

A conversão `graus × π / 180` transforma graus em radianos, que é a unidade usada pelas funções trigonométricas de C (`sin`, `cos`, etc.).

---

## Fluxo Principal — `main.c`

### `main()`

```
argc == 2?  ──Não──► erro + exit
     │
    Sim
     │
ft_memset(&data, 0)   ← zera toda a struct antes de usar
     │
init_data()
     │
game_render()         ← renderiza o primeiro frame
     │
mlx_loop()            ← entra no loop de eventos da MiniLibX
```

`ft_memset(&data, 0, sizeof(t_data))` garante que todos os campos começam a zero/NULL antes de qualquer inicialização. Isto evita comportamento indefinido caso algum ponteiro fique por inicializar.

---

### `init_data()`

Inicializa todos os subsistemas pela seguinte ordem:

1. **`parse_map()`** — lê e valida o ficheiro `.cub`
2. **`mlx_init()`** — inicializa a ligação ao servidor gráfico
3. **`mlx_new_window()`** — cria a janela
4. **`mlx_new_image()`** — cria o buffer de imagem off-screen
5. **`mlx_get_data_addr()`** — obtém o ponteiro para os pixels do buffer
6. **`init_player()`** — define posição e vetores do jogador
7. **`events_init()`** — regista os callbacks de teclado

A ordem importa: não faz sentido criar a janela se o mapa falhou, nem obter o endereço da imagem antes de a criar.

O padrão de retorno antecipado com `print_message_and_free` garante que em caso de erro qualquer recurso já alocado é libertado antes de sair.

---

## Parsing — `parsing.c`

O objetivo deste ficheiro é ler um ficheiro `.cub` e preencher a struct `t_map`.

### `count_lines()`

```c
static int count_lines(char *filename)
```

Abre o ficheiro e conta o número de linhas usando `get_next_line`. É chamada **duas vezes** em `parse_map`: uma para verificar se o ficheiro tem linhas, e outra para obter o valor real para `map->height`.

> **Nota**: Esta duplicação (abrir o ficheiro duas vezes) é uma decisão consciente para manter as funções simples e separadas. Uma alternativa seria guardar o resultado na primeira chamada, mas optou-se pela legibilidade.

---

### `calc_width()`

```c
int calc_width(t_map *map)
```

Percorre todas as linhas do mapa e devolve o comprimento da linha mais longa. É usada em vez de `ft_strlen(map->grid[0])` porque o mapa pode ter linhas de comprimentos diferentes — a primeira linha não é necessariamente a mais larga.

```
Exemplo:
  "111111"       ← 6 chars
  "100001"       ← 6 chars
  "1001"         ← 4 chars  (linha mais curta)
  "111111"       ← 6 chars

  map->width = 6
```

---

### `fill_map()`

```c
static t_map *fill_map(t_map *map, char *filename)
```

Lê o ficheiro linha a linha com `get_next_line` e armazena cada linha em `map->grid[i]`. Antes de guardar, remove os caracteres `\n` e `\r` do fim de cada linha:

```c
while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
{
    line[len - 1] = '\0';
    len--;
}
```

O `\r` é removido para compatibilidade com ficheiros criados no Windows (que terminam as linhas com `\r\n` em vez de apenas `\n`).

---

### `parse_map()`

Orquestra todo o parsing:

```
malloc(t_map)
      │
count_lines()  →  map->height
      │
malloc(map->grid)
      │
fill_map()     →  preenche map->grid[]
      │
check_if_map_is_valid()
      │
devolve map ou NULL (com limpeza de memória)
```

---

## Validação do Mapa — `map_checker_1.c`

### `check_name()`

Verifica que o ficheiro termina em `.cub`:

```c
cub = ft_strrchr(filename, '.');  // encontra o último '.'
ft_strcmp(cub, ".cub") != 0       // compara com ".cub"
```

`ft_strrchr` (last char) é usada em vez de `ft_strchr` para lidar com paths que contenham pontos noutros sítios, como `./maps/my.map.cub` — o que nos interessa é a extensão final.

---

### `check_format()`

Garante que o mapa tem pelo menos 3×3 tiles. Um mapa 2×2 seria tecnicamente fechado mas não jogável.

---

### `check_characters()`

Percorre todos os caracteres do mapa e verifica:

| Caráter | Significado |
|---------|-------------|
| `0`     | Espaço livre |
| `1`     | Parede |
| ` `     | Espaço vazio (fora do mapa) |
| `N/S/E/W` | Posição + direção inicial do jogador |
| `A`     | Posição da saída (exit) |

**Regras impostas:**
- Exatamente **1** jogador (`N`, `S`, `E`, ou `W`)
- Exatamente **1** saída (`A`)
- Qualquer outro caráter → mapa inválido

Quando encontra o jogador ou a saída, guarda as coordenadas na struct e **substitui o caráter por `'0'`** no grid. Isto simplifica o raycasting: o mapa fica apenas com `0`, `1` e ` `, sem casos especiais.

---

### `check_if_map_is_valid()`

Função principal de validação — chama as verificações por ordem e devolve 0 ao primeiro erro:

```
check_name()        → extensão .cub?
check_format()      → dimensões mínimas?
check_characters()  → caracteres válidos + 1 jogador + 1 saída?
check_borders()     → mapa fechado? (flood fill)
```

---

## Flood Fill — `map_checker_2.c`

### O Problema

Como verificar que o mapa está completamente fechado por paredes? Isto é, que o jogador nunca consegue "escapar" para fora dos limites?

A abordagem naive seria verificar se todas as células do bordo são paredes — mas um mapa pode ser não-convexo, com paredes internas e corredores complexos.

A solução robusta é o **flood fill**: simular a propagação a partir da posição do jogador e verificar se essa propagação alguma vez ultrapassa os limites do mapa.

---

### `is_valid()`

```c
static int is_valid(t_map *map, int x, int y, int line_len)
```

Devolve `0` (inválido) se:
- `y` está fora dos limites verticais do mapa
- `x` está fora dos limites horizontais **da linha atual** (não do mapa global)
- O caráter na posição é `' '` (espaço vazio)

> **Porquê usar `ft_strlen(map->grid[y])` em vez de `map->width`?**  
> As linhas do mapa têm comprimentos diferentes. Uma linha pode ter 10 caracteres e outra 6. Se usássemos `map->width` (o máximo global), estaríamos a aceder a memória fora da string em linhas mais curtas — comportamento indefinido. Por isso, em cada chamada recursiva, calculamos o comprimento real da linha atual.

---

### `flood_fill()` — O Algoritmo

```c
static void flood_fill(t_map *map, int x, int y, int *escaped)
```

Este é um algoritmo recursivo de preenchimento em 4 direções (cima, baixo, esquerda, direita). Funciona assim:

**Condições de paragem:**
1. `*escaped == 1` → já sabemos que escapou, não continuar
2. `!is_valid(...)` → saímos dos limites → **marca como escapado** (`*escaped = 1`)
3. `grid[y][x] == '1'` → é parede, não atravessar
4. `grid[y][x] == 'V'` → já visitado, não revisitar (evita loop infinito)

**Passo recursivo:**
```
marca grid[y][x] = 'V'   (visited)
→ flood_fill(x+1, y)     direita
→ flood_fill(x-1, y)     esquerda
→ flood_fill(x, y+1)     baixo
→ flood_fill(x, y-1)     cima
```

**Porquê marcar com `'V'`?**  
Sem esta marcação, o algoritmo visitaria a mesma célula infinitas vezes (A visita B, B visita A, loop infinito). O `'V'` funciona como memória: "já passei aqui".

**Porquê `int *escaped` (ponteiro) em vez de retornar um valor?**  
Como a função é recursiva, retornar um valor não seria suficiente para "parar tudo". Com um ponteiro partilhado, assim que uma chamada detecta que escapou, todos os ramos da recursão verificam `if (*escaped) return;` e terminam imediatamente — é uma forma eficiente de curto-circuitar a recursão.

---

### Visualização do Flood Fill

```
Mapa original (jogador em [1][1]):        Após flood fill:

  1 1 1 1 1                                 1 1 1 1 1
  1 0 0 0 1                                 1 V V V 1
  1 0 1 0 1         ──────────►             1 V 1 V 1
  1 0 0 0 1                                 1 V V V 1
  1 1 1 1 1                                 1 1 1 1 1

  Resultado: não escapou → mapa válido ✓
```

```
Mapa com buraco:

  1 1 1 1 1                                 1 1 1 1 1
  1 0 0 0 1                                 1 V V V 1
  1 0   0 1         ──────────►             1 V   V 1  ← espaço em branco
  1 0 0 0 1                                 escaped = 1 !
  1 1 1 1 1

  Resultado: escapou → mapa inválido ✗
```

---

### `check_flood_fill_result()`

Após o flood fill, verifica se ficaram células `'0'` não visitadas (e também `A`, `N`, `S`, `E`, `W` caso não tenham sido substituídas). Se existirem, significa que há zonas do mapa inacessíveis a partir do jogador — o que pode indicar um erro no mapa.

> **Nota**: Esta função é chamada com `result = !check_flood_fill_result(copy)` — o resultado é invertido porque a função devolve `1` se encontrou células não visitadas (problema), e queremos `result = 1` para mapa válido.

---

### `check_borders()`

```c
int check_borders(t_map *map)
```

Orquestra a verificação de bordas:

1. **Copia o mapa** com `copy_map()` — o flood fill modifica o grid (escreve `'V'`), por isso trabalhamos sempre numa cópia para não corromper o mapa original.
2. Corre `flood_fill()` a partir da posição inicial do jogador.
3. Se `escaped == 1` → inválido.
4. Se ficaram células não visitadas → inválido.
5. Limpa a cópia com `clean_map()`.

---

---

## Eventos e Loop de Jogo — `event.c`

### `ft_pixel_put()`

```c
void ft_pixel_put(int x, int y, t_img *img, int color)
```

Escreve um pixel de cor `color` na posição `(x, y)` do buffer de imagem. É a função base de toda a renderização — tudo o que aparece no ecrã passa por aqui.

**A fórmula do offset:**
```c
offset = (y * img->line_len) + (x * (img->bpp / 8));
*(unsigned int *)(img->addr + offset) = color;
```

A memória da imagem é um array linear de bytes. Para chegar ao pixel `(x, y)`:
- `y * line_len` → avança `y` linhas completas (em bytes)
- `x * (bpp / 8)` → avança `x` pixels dentro da linha (`bpp / 8` converte bits em bytes; com 32bpp, cada pixel ocupa 4 bytes)

O cast `*(unsigned int *)` escreve os 4 bytes da cor de uma só vez.

**A guarda dos limites:**
```c
if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
    return;
```
Sem esta verificação, um pixel fora dos limites corromperia memória adjacente ao buffer. Toda a função de renderização usa `ft_pixel_put` diretamente, por isso esta guarda centraliza a proteção num único sítio.

---

### `game_render()`

```c
void game_render(t_data *data)
{
    render_3d(data);
    draw_map(data);
    render_minimap_rays(data);
    draw_player(data);
    mlx_put_image_to_window(...);
}
```

A ordem das chamadas define a ordem de "pintura" — cada camada sobrepõe a anterior:

```
render_3d()              ← pinta o fundo: vista 3D (todo o ecrã)
draw_map()               ← por cima: minimapa 2D (canto superior esquerdo)
render_minimap_rays()    ← por cima: raios do raycasting no minimapa
draw_player()            ← por cima: círculo rosa do jogador
mlx_put_image_to_window  ← envia o buffer completo para a janela (um único flush)
```

O facto de só existir um `mlx_put_image_to_window` no final é intencional: todo o frame é construído no buffer off-screen e enviado para o ecrã de uma vez. Isto elimina o flickering que aconteceria se enviássemos cada elemento individualmente.

---

### `game_loop()`

```c
int game_loop(t_data *data)
{
    if (data->keys[KEY_W]) move_player(data,  SPEED, 0.0f);
    if (data->keys[KEY_S]) move_player(data, -SPEED, 0.0f);
    if (data->keys[KEY_A]) move_player(data,  0.0f, -SPEED);
    if (data->keys[KEY_D]) move_player(data,  0.0f,  SPEED);
    game_render(data);
    return (0);
}
```

Registado com `mlx_loop_hook`, esta função é chamada pela MiniLibX em cada iteração do loop de eventos — isto é, o mais rápido possível (sem cap de FPS).

**Porquê `keys[]` em vez de reagir diretamente no `key_press_handler`?**

Se o movimento fosse feito no handler de keypress, o jogador só avançaria um passo por cada evento de tecla. O sistema operativo tem um delay entre o primeiro evento e a repetição automática, o que causaria movimento irregular (avança → pausa → avança rápido).

Com o array `keys[]`, o padrão é:
- `key_press` → `keys[KEY_W] = 1`
- `game_loop` (cada frame) → vê `keys[KEY_W] == 1` → move
- `key_release` → `keys[KEY_W] = 0`

O resultado é movimento contínuo e suave enquanto a tecla estiver premida.

---

### `events_init()`

```c
void events_init(t_data *data)
{
    mlx_hook(data->win, KeyPress,      KeyPressMask,      key_press_handler,   data);
    mlx_hook(data->win, KeyRelease,    KeyReleaseMask,    key_release_handler, data);
    mlx_hook(data->win, DestroyNotify, StructureNotifyMask, close_handler,     data);
    mlx_loop_hook(data->mlx, game_loop, data);
}
```

Regista 4 callbacks:
- **KeyPress / KeyRelease** — atualizam o array `keys[]`
- **DestroyNotify** — captura o clique no botão ✕ da janela e chama `close_handler`
- **mlx_loop_hook** — o `game_loop`, chamado em cada frame

Sem o `DestroyNotify`, fechar a janela pelo botão ✕ não terminaria o processo — o programa continuaria a correr em background.

---

## Input de Teclado — `key_handler.c`

### `key_press_handler()`

Distingue dois tipos de teclas:

**Teclas de estado** (W/A/S/D) — ativam um flag:
```c
data->keys[KEY_W] = 1;
```
O movimento acontece no `game_loop`, não aqui.

**Teclas de ação imediata** (setas, h/l, Escape) — executam diretamente:
```c
rotate_player(data, -data->player.rot_speed);  // seta esquerda ou 'h'
rotate_player(data,  data->player.rot_speed);  // seta direita ou 'l'
close_handler(data);                            // Escape
```

A rotação é de ação imediata porque é ativada por eventos de keypress individuais. As teclas `h` e `l` são um bónus de estilo vim para rotação.

---

### `key_release_handler()`

Apenas zera os flags W/A/S/D:
```c
data->keys[KEY_W] = 0;
```

As teclas de rotação (setas) não têm estado — cada evento de press roda um incremento fixo.

---

### `close_handler()`

Liberta todos os recursos pela ordem inversa da criação, e termina o processo:

```
mlx_destroy_image()    ← liberta o buffer de pixels
mlx_destroy_window()   ← fecha a janela
clean_map()            ← liberta o grid e a struct do mapa
mlx_destroy_display()  ← fecha a ligação ao servidor X11
free(data->mlx)        ← liberta a struct MLX
exit(EXIT_SUCCESS)
```

A ordem importa: não se pode destruir o display antes de destruir a janela e a imagem que dependem dele.

> **Nota no código**: o comentário sobre `mlx_destroy_image` estava a questionar se era necessário — é. A imagem é alocada separadamente da janela e precisa de ser libertada explicitamente para evitar memory leaks.

---

## Minimapa 2D — `minimap.c`

O minimapa é uma representação visual do mapa em perspetiva top-down, desenhada no canto superior esquerdo do ecrã por cima da vista 3D.

### `is_wall()`

```c
int is_wall(t_data *data, float x, float y)
```

Verifica se a posição `(x, y)` em coordenadas de mapa é uma parede. Recebe floats e converte para inteiros com `(int)x` — ou seja, trunca para o tile em que a posição se encontra.

Considera parede se:
- Fora dos limites do mapa → retorna `1` (trata o exterior como parede)
- O caráter no tile é `'1'`

Esta função é partilhada com o sistema de colisão.

---

### `collision()`

```c
int collision(t_data *data, float new_x, float new_y)
```

Verifica se a nova posição `(new_x, new_y)` colidiria com uma parede. Em vez de verificar apenas o ponto central do jogador, verifica **4 cantos** de uma "hitbox" quadrada com margem `m = 0.2f`:

```
(x-m, y-m)  ·····  (x+m, y-m)
     ·                    ·
     ·         @          ·
     ·      jogador       ·
     ·                    ·
(x-m, y+m)  ·····  (x+m, y+m)
```

**Porquê 4 cantos e não 1 ponto central?**

Com apenas o ponto central, o jogador conseguiria atravessar cantos de paredes parcialmente — o centro estaria em espaço livre mas visualmente o jogador sobrepunha-se à parede. Os 4 cantos garantem que nenhuma parte da "cápsula" do jogador entra numa parede.

O valor `0.2f` foi escolhido empiricamente: grande o suficiente para evitar sobreposição visual, pequeno o suficiente para passar por corredores de 1 tile de largura.

---

### `draw_tile()`

```c
static void draw_tile(t_data *data, int x, int y, int color)
```

Pinta um tile inteiro de cor `color`. Como cada tile tem `PX × PX` pixels (32×32), o loop percorre todos esses pixels e chama `ft_pixel_put` para cada um:

```c
ft_pixel_put(x * PX + px, y * PX + py, &data->img, color);
```

`x * PX` converte coordenadas de tile para coordenadas de pixel no ecrã.

---

### `draw_map()`

Percorre o grid e chama `draw_tile` com a cor certa para cada tile:

| Caráter | Cor | Significado |
|---------|-----|-------------|
| `'1'`   | `WHITE` | Parede |
| `'0'`   | `UBUNTU` (roxo) | Chão livre |
| outro   | `BLACK` | Espaço vazio / fora do mapa |

---

## Jogador — `player.c`

### `init_player()`

```c
data->player.x = data->map->player_initial_x + 0.5f;
data->player.y = data->map->player_initial_y + 0.5f;
```

O `+ 0.5f` centra o jogador no tile. As coordenadas do mapa são inteiros (coluna/linha), mas a posição do jogador é o centro do tile, por isso somamos metade de um tile.

**Os vetores `dir` e `plane`:**

O raycaster usa dois vetores para definir a câmara:
- **`dir`**: direção para onde o jogador olha (comprimento 1)
- **`plane`**: o plano da câmara — perpendicular a `dir`, com comprimento `PLANE_LEN`

`PLANE_LEN` é calculado a partir do FOV:
```
PLANE_LEN = tan(FOV / 2)
```
Com FOV = 60°: `tan(30°) ≈ 0.577`

A relação entre `dir` e `plane` determina o FOV: quanto maior `plane` em relação a `dir`, maior o ângulo de visão.

Para cada direção inicial, os vetores são perpendiculares:

| Direção | `dir`     | `plane`              |
|---------|-----------|----------------------|
| Norte   | (0, -1)   | (PLANE_LEN, 0)       |
| Sul     | (0, +1)   | (-PLANE_LEN, 0)      |
| Este    | (+1, 0)   | (0, PLANE_LEN)       |
| Oeste   | (-1, 0)   | (0, -PLANE_LEN)      |

O sinal do `plane` garante que a câmara aponta "para a direita" relativamente à direção do jogador. Trocar o sinal inverteria a imagem horizontalmente.

---

### `move_player()`

```c
void move_player(t_data *data, float forward, float strafe)
```

Recebe dois valores:
- `forward`: intensidade do movimento para a frente/trás (`+SPEED` ou `-SPEED`)
- `strafe`: intensidade do movimento lateral (`+SPEED` ou `-SPEED`)

**Cálculo do vetor de movimento:**

```c
// Movimento para a frente: na direção de dir
move_x += data->player.dir_x * speed * forward;
move_y += data->player.dir_y * speed * forward;

// Movimento lateral (strafe): perpendicular a dir
// O vetor perpendicular a (dir_x, dir_y) é (-dir_y, dir_x)
move_x += -data->player.dir_y * speed * strafe;
move_y +=  data->player.dir_x * speed * strafe;
```

O vetor perpendicular a `(dir_x, dir_y)` é `(-dir_y, dir_x)` — rotação de 90° no sentido anti-horário. Isto garante que o strafe é sempre lateral à direção atual do jogador, independentemente para onde está a olhar.

**Colisão independente por eixo:**

```c
if (!collision(data, data->player.x + move_x, data->player.y))
    data->player.x += move_x;
if (!collision(data, data->player.x, data->player.y + move_y))
    data->player.y += move_y;
```

Os eixos X e Y são verificados **separadamente**. Se o jogador colide com uma parede em diagonal, pode continuar a deslizar ao longo dela no eixo livre. Sem esta separação, ao encostar a uma parede em diagonal o jogador ficaria completamente bloqueado, o que seria frustrante.

---

### `rotate_player()`

```c
void rotate_player(t_data *data, float angle)
```

Aplica uma rotação de `angle` radianos ao vetor `dir` e ao vetor `plane` usando a **matriz de rotação 2D**:

```
[ cos(a)  -sin(a) ] × [ dir_x ]   =   [ dir_x * cos(a) - dir_y * sin(a) ]
[ sin(a)   cos(a) ]   [ dir_y ]       [ dir_x * sin(a) + dir_y * cos(a) ]
```

No código:
```c
old_dir_x = data->player.dir_x;   // guarda antes de sobrescrever!

data->player.dir_x = dir_x * cos_a - dir_y * sin_a;
data->player.dir_y = old_dir_x * sin_a + dir_y * cos_a;
```

> **Porquê guardar `old_dir_x`?**  
> O novo `dir_y` precisa do `dir_x` original. Se não o guardarmos antes de calcular o novo `dir_x`, o cálculo de `dir_y` usaria o valor já atualizado — resultado incorreto.

O mesmo processo aplica-se ao vetor `plane` (com `old_plane_x`). É crucial que `dir` e `plane` rodem **sempre juntos e com o mesmo ângulo** — caso contrário o FOV distorce-se progressivamente a cada rotação.

**A rotação preserva o comprimento dos vetores?**

Sim. A matriz de rotação é ortogonal — não escala os vetores, apenas os roda. Por isso não é necessário re-normalizar `dir` ou `plane` após cada rotação.

---

### `draw_player()`

Desenha o jogador como um círculo rosa no minimapa. O raio é `PX / 2 - 10`.

**Algoritmo do círculo (sem `sqrt`):**

Em vez de calcular `sqrt(dx² + dy²) <= r`, usa-se a condição equivalente `dx² + dy² <= r²` — evita a raiz quadrada, que é mais lenta:

```c
if (dx * dx + dy * dy <= r * r)
    ft_pixel_put(cx + dx, cy + dy, &data->img, PINK);
```

O centro do círculo em pixels é `(player.x * PX, player.y * PX)` — converte coordenadas de tile para pixels multiplicando por `PX` (32).

---

---

## Raycasting — `raycasting.c`

Esta é a parte central do engine. Para cada coluna de pixels do ecrã (de `x = 0` a `x = WIDTH - 1`), lançamos um raio, calculamos a que distância bate numa parede, e desenhamos uma fatia vertical proporcional a essa distância.

O fluxo é sempre:
```
para cada coluna x:
    init_ray()      → define a direção do raio e os dados DDA
    perform_dda()   → avança o raio até bater numa parede
    draw_column()   → desenha teto + fatia de parede + chão
```

---

### Conceito Base: o Plano da Câmara

Antes de entrar no código, é essencial perceber o modelo geométrico:

```
                        plane
                   ←————————————→
                   E            D
                    \           |
                     \          |
                      \         |
                       \        |  dir
                        \       |
                         \      ↓
                          \   [jogador]
                           \
                            (raio da coluna x=0)
```

O jogador tem:
- **`dir`**: vetor unitário para onde olha
- **`plane`**: vetor perpendicular, representando o "ecrã" virtual

O ecrã virtual tem largura 2 (de -1.0 a +1.0). Cada coluna `x` corresponde a um ponto neste ecrã virtual, chamado `camera_x`.

---

### `init_ray()`

#### Passo 1 — `camera_x`: posição da coluna no ecrã virtual

```c
float camera_x = 2.0f * x / WIDTH - 1.0f;
```

Converte a coluna de pixel `x` (0 a WIDTH-1) para o intervalo [-1.0, +1.0]:

```
x = 0          → camera_x = -1.0   (borda esquerda)
x = WIDTH/2    → camera_x =  0.0   (centro)
x = WIDTH-1    → camera_x ≈ +1.0   (borda direita)
```

#### Passo 2 — Direção do raio

```c
ray->ray_dir_x = p->dir_x + p->plane_x * camera_x;
ray->ray_dir_y = p->dir_y + p->plane_y * camera_x;
```

O raio aponta do jogador para o ponto correspondente no plano da câmara. É a soma do vetor `dir` (centro do ecrã) com uma fração do vetor `plane` (deslocamento lateral):

```
camera_x = 0.0  → raio central  = dir + plane * 0   = dir
camera_x = -1.0 → raio esquerdo = dir + plane * (-1) = dir - plane
camera_x = +1.0 → raio direito  = dir + plane * (+1) = dir + plane
```

#### Passo 3 — `delta_dist`: distância entre cruzamentos de grid

```c
ray->delta_dist_x = fabsf(1.0f / ray->ray_dir_x);
ray->delta_dist_y = fabsf(1.0f / ray->ray_dir_y);
```

`delta_dist_x` é a distância que o raio percorre entre dois cruzamentos verticais consecutivos da grelha (linhas x = 0, x = 1, x = 2, ...). Geometricamente:

```
Se o raio tem direção (dx, dy), a distância entre dois cruzamentos
verticais consecutivos é 1/|dx| — porque para avançar 1 unidade em X,
o raio percorre hipotenusa = 1/|dx| * comprimento_total_do_vetor.

Como usamos vetores normalizados (|dir| = 1 ou próximo), simplifica para:
    delta_dist_x = |1 / ray_dir_x|
```

Visualmente:
```
    |       |       |
    |  raio |       |
    |   /   |       |
    |  /←d→ |       |      d = delta_dist_x
    | /     |       |
    |/      |       |
    +-------+-------+
```

Se `ray_dir_x == 0`, o raio é paralelo às linhas verticais e nunca as cruza — por isso usamos `1e30f` (infinito prático) para esse eixo.

#### Passo 4 — `step` e `side_dist`: arranque do DDA

`step_x` / `step_y` indicam em que direção o raio avança (+1 ou -1).

`side_dist_x` / `side_dist_y` são a distância do jogador até ao **primeiro** cruzamento de grid em cada eixo:

```c
// raio vai para a esquerda (ray_dir_x < 0):
ray->step_x = -1;
ray->side_dist_x = (p->x - ray->map_x) * ray->delta_dist_x;
//                  ↑ fração do tile atual até à borda esquerda
```

```c
// raio vai para a direita (ray_dir_x > 0):
ray->step_x = +1;
ray->side_dist_x = (ray->map_x + 1.0f - p->x) * ray->delta_dist_x;
//                  ↑ fração do tile atual até à borda direita
```

Visualmente, com o jogador a 0.3 tiles da borda esquerda do tile atual:
```
    tile atual
    |←——0.3——→|←—— 0.7 ——→|
    |          |            |
    |    @     |            |
    |←—————————→            |
    side_dist_x = 0.7 * delta_dist_x    (raio vai para a direita)
```

---

### `perform_dda()` — Digital Differential Analysis

O DDA avança o raio cruzamento a cruzamento até bater numa parede. A ideia é simples: a cada passo, avançamos pelo cruzamento mais próximo (X ou Y).

```c
while (!hit)
{
    if (ray->side_dist_x < ray->side_dist_y)
    {
        ray->side_dist_x += ray->delta_dist_x;  // próximo cruzamento X
        ray->map_x += ray->step_x;              // avança um tile em X
        ray->side = 0;                           // bateu em face N/S
    }
    else
    {
        ray->side_dist_y += ray->delta_dist_y;
        ray->map_y += ray->step_y;
        ray->side = 1;                           // bateu em face E/W
    }
    if (is_wall(...)) hit = 1;
}
```

**Visualização passo a passo:**

```
Iteração 1: side_dist_x=0.4 < side_dist_y=0.7 → avança em X
            side_dist_x passa a 0.4 + delta_x

Iteração 2: side_dist_x=1.1 > side_dist_y=0.7 → avança em Y
            side_dist_y passa a 0.7 + delta_y

Iteração 3: ... até encontrar '1' no grid
```

O algoritmo garante que nunca "salta por cima" de um tile — avança sempre pelo cruzamento mais próximo, tile a tile.

#### Porquê `side == 0` é face N/S e `side == 1` é face E/W?

- `side == 0` → o raio cruzou uma linha vertical da grelha → bateu numa face que é vertical → face Norte ou Sul (perpendicular ao eixo X)
- `side == 1` → o raio cruzou uma linha horizontal → bateu numa face Este ou Oeste (perpendicular ao eixo Y)

#### A distância perpendicular — corrigir o efeito fisheye

```c
if (ray->side == 0)
    ray->perp_dist = ray->side_dist_x - ray->delta_dist_x;
else
    ray->perp_dist = ray->side_dist_y - ray->delta_dist_y;
```

Não podemos usar a distância euclidiana direta (linha reta do jogador à parede). Isso causaria o **efeito fisheye**: as paredes pareceriam curvas porque os raios das bordas do ecrã percorrem mais distância do que o raio central.

```
Distância euclidiana:           Distância perpendicular:
    ╲  |  ╱                         |   |   |
     ╲ | ╱   ← raios mais           |   |   |   ← fatias uniformes
      ╲|╱      longos nas           |   |   |
       @        bordas              @
    (fisheye!)                  (correto)
```

A distância perpendicular é a distância ao **plano da câmara** (não ao ponto de impacto). Obtemo-la recuando um `delta_dist` a partir do `side_dist` final — porque no momento do `hit`, o `side_dist` já foi incrementado para o cruzamento *seguinte*:

```
antes do hit:   side_dist_x = distância ao cruzamento onde bateu + delta_dist_x
                                                                    ↑ já incrementado
portanto:       perp_dist = side_dist_x - delta_dist_x
```

---

### `draw_column()`

Com a `perp_dist` calculada, sabemos a que distância está a parede. A altura da fatia é inversamente proporcional à distância:

```c
line_height = (int)(HEIGHT / ray->perp_dist);
```

- Parede perto → `perp_dist` pequeno → `line_height` grande → fatia alta
- Parede longe → `perp_dist` grande → `line_height` pequeno → fatia baixa

As coordenadas de início e fim da fatia são centradas verticalmente:

```c
draw_start = HEIGHT / 2 - line_height / 2;
draw_end   = HEIGHT / 2 + line_height / 2;
```

Se `line_height > HEIGHT` (parede muito próxima), o clamp garante que não saímos do ecrã:
```c
if (draw_start < 0)        draw_start = 0;
if (draw_end >= HEIGHT)    draw_end = HEIGHT - 1;
```

**Cores e sombreamento:**

```c
if (ray->side == 1)  color = 0x00AAAAAA;  // faces E/W — mais escuras
else                 color = 0x00FFFFFF;  // faces N/S — mais claras
```

Esta distinção de brilho entre faces cria uma sensação de iluminação direcional sem nenhum cálculo de luz real — uma técnica clássica dos raycasters para dar profundidade à cena.

Cada coluna é pintada em 3 zonas:

```
y = 0
  ┌──────────┐  ← teto (cinzento escuro 0x333333)
  ├──────────┤  ← draw_start
  │          │
  │  parede  │  ← branco ou cinzento (consoante side)
  │          │
  ├──────────┤  ← draw_end
  └──────────┘  ← chão (cinzento médio 0x555555)
y = HEIGHT
```

---

### `render_3d()` e `render_minimap_rays()`

Ambas percorrem as `WIDTH` colunas e chamam `init_ray` + `perform_dda`. A diferença é apenas no passo final:

- `render_3d` → chama `draw_column` → projeta a parede no ecrã 3D
- `render_minimap_rays` → chama `draw_ray_minimap` → desenha o raio no minimapa 2D

Reutilizar `init_ray` e `perform_dda` para ambos é uma decisão de design importante: o mesmo raio que define o que se vê em 3D é também o que se visualiza no minimapa — consistência garantida.

---

---

## Raios no Minimapa — `draw_ray_minimap()`

```c
void draw_ray_minimap(t_data *data, t_ray *ray, int x)
```

Desenha um raio individual no minimapa 2D, partindo do jogador até ao ponto de impacto na parede.

### Otimização: só desenha 1 em cada 8 raios

```c
if (x % 8 != 0)
    return;
```

São lançados `WIDTH` (1024) raios por frame. Desenhar todos no minimapa seria visualmente idêntico a desenhar 1 em cada 8 — a resolução do minimapa (em pixels) não distingue raios adjacentes. Esta guarda reduz o número de chamadas a `ft_pixel_put` nesta função por um fator de 8, sem impacto visual.

### O loop de avanço

```c
ray_x = data->player.x;
ray_y = data->player.y;
step  = 0.03f;
dist  = 0.0f;

while (dist < ray->perp_dist)
{
    ft_pixel_put((int)(ray_x * PX), (int)(ray_y * PX), &data->img, RED);
    ray_x += ray->ray_dir_x * step;
    ray_y += ray->ray_dir_y * step;
    dist  += step;
}
```

O raio é desenhado pixel a pixel, avançando em passos de `0.03f` tiles na direção `ray_dir`. A cada passo:
- Converte coordenadas de tile para pixels (`* PX`) e pinta de vermelho
- Para quando `dist >= perp_dist` — ou seja, quando chegou à parede

**Porquê `step = 0.03f`?**  
Um step maior saltaria pixels visíveis (raio com "buracos"). Um step menor seria mais trabalho sem ganho visual. Com `PX = 32` pixels por tile, um step de `0.03f` tiles ≈ 1 pixel por iteração — o granulo ideal.

**Porquê usar `ray_dir` diretamente em vez de um vetor normalizado?**  
O `ray_dir` não é unitário (o seu comprimento varia com o FOV), mas é consistente: a `perp_dist` foi calculada com base no mesmo `ray_dir`. Por isso a condição `dist < perp_dist` continua correta — as duas grandezas estão na mesma escala.

---

## Utilitários — `utils.c`

### `clean_map()`

```c
void clean_map(t_map *map)
```

Liberta toda a memória associada ao mapa, pela ordem correta:

```
for cada linha → free(map->grid[i])   ← liberta o conteúdo de cada string
free(map->grid)                        ← liberta o array de ponteiros
free(map)                              ← liberta a struct
```

A guarda `if (!map) return` e `if (map->grid)` tornam a função segura para chamadas com ponteiros NULL — útil em casos de erro a meio da inicialização, onde o mapa pode estar parcialmente alocado.

---

### `print_message_and_free()`

```c
void print_message_and_free(char *message, t_data *data, int fd)
```

Centraliza a lógica de "imprimir erro e limpar" que é repetida em vários pontos de `init_data` e `main`. Evita duplicar o `clean_map` em cada `return` de erro.

O `fd` permite escrever para `stdout` (1) ou `stderr` (2) conforme o contexto — os erros são enviados para `stderr` (fd = 2), seguindo a convenção Unix.

> **Nota**: esta função não chama `exit` nem liberta `data->mlx`, `data->win`, ou `data->img` — apenas o mapa. A libertação dos recursos MLX é responsabilidade de `close_handler`. Em contextos de erro precoce (antes de `mlx_init`), esses ponteiros ainda são NULL, por isso não há leak.

---

### `copy_map()`

```c
t_map *copy_map(t_map *original)
```

Cria uma cópia profunda (*deep copy*) do mapa. É usada em `check_borders()` antes de correr o flood fill, que modifica o grid ao escrever `'V'`.

**Porquê deep copy e não shallow copy?**

Uma shallow copy (`*copy = *original`) copiaria os ponteiros — `copy->grid` e `original->grid` apontariam para o mesmo array de strings. O flood fill ao modificar `copy->grid[i][j]` estaria a modificar o mapa original, corrompendo-o para o resto do programa.

A deep copy duplica cada string com `ft_strdup`, garantindo que as duas structs são completamente independentes:

```
original->grid[0] → "11111\0"     (string A)
copy->grid[0]     → "11111\0"     (string B — cópia independente)

flood fill escreve 'V' em copy:
copy->grid[0]     → "1VVV1\0"     ← modificada
original->grid[0] → "11111\0"     ← intacta ✓
```

O `NULL` no final (`copy->grid[copy->height] = NULL`) mantém a convenção de array terminado a NULL, necessária para que os loops `while (map->grid[i])` funcionem corretamente na cópia.

---

## Estado Atual do Projeto

O projeto está funcional com os seguintes sistemas implementados:

| Sistema | Ficheiros | Estado |
|---|---|---|
| Parsing & Validação | `parsing.c`, `map_checker_1.c`, `map_checker_2.c` | ✅ Completo |
| Loop principal & eventos | `main.c`, `event.c`, `key_handler.c` | ✅ Completo |
| Minimapa 2D | `minimap.c` | ✅ Completo |
| Jogador (movimento, rotação, colisão) | `player.c` | ✅ Completo |
| Raycasting & renderização 3D | `raycasting.c` | ✅ Completo |
| Utilitários & memória | `utils.c` | ✅ Completo |
| Texturas nas paredes | — | 🔲 Por implementar |
| Cores de teto/chão por mapa | — | 🔲 Por implementar |

---

*Documento atualizado até à versão atual do projeto. Novas secções serão adicionadas à medida que o projeto evolui.*