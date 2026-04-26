# Contexto de Desenvolvimento - Gnome Themes Manager

Este arquivo serve como o registro de memória do projeto, conforme definido no `PROJECT_SPEC.md`.

## Estado Atual
- [x] Definição de requisitos e especificações no `PROJECT_SPEC.md`.
- [x] Criação do script de dependências `setup_deps.sh`.
- [ ] Configuração do ambiente de build (Meson/Ninja).
- [ ] Implementação do módulo de identificação de configurações (GSettings).
- [ ] Implementação da lógica de backup (Compactação de ativos).
- [ ] Desenvolvimento da interface TUI.

## Decisões de Arquitetura (ADR)

### 001: Uso de GLib e GIO
- **Contexto:** Necessidade de acessar configurações do GNOME de forma robusta.
- **Decisão:** Utilizar as APIs oficiais (`g_settings_*`) em vez de manipular arquivos de texto diretamente.
- **Consequência:** Garante compatibilidade futura e simplifica o acesso a chaves do dconf.

### 002: Instalação Local de Temas Globais
- **Contexto:** Restaurar temas de sistema em máquinas onde o usuário não tem permissão de escrita em `/usr`.
- **Decisão:** Se o tema for global e estiver faltando, restaurar para `~/.themes` ou `~/.local/share/icons`.
- **Consequência:** Aumenta a portabilidade do backup sem exigir privilégios de administrador.

### 003: Sistema de Erros Bitwise
- **Contexto:** Necessidade de reportar falhas de forma granular (ex: falha parcial em um backup).
- **Decisão:** Definir um `enum` onde cada erro é uma potência de 2 (1 << n).
    - `ERR_GSETTINGS (1 << 0)`
    - `ERR_FILESYSTEM (1 << 1)`
    - `ERR_COMPRESSION (1 << 2)`
    - `ERR_PERMISSION (1 << 3)`
- **Consequência:** Permite que a função retorne `ERR_FILESYSTEM | ERR_PERMISSION` e a TUI mostre mensagens específicas para cada bit ativo.

### 004: Otimização de Memória e Alinhamento (Padding)
- **Contexto:** Minimizar ciclos de CPU e otimizar o uso do cache durante o tratamento de erros e manipulação de estados.
- **Decisão:** 
    - Utilizar `uint64_t` ou `uint32_t` explícitos para máscaras de bits para garantir tamanho consistente.
    - Aplicar `alignas` (C11) ou atributos de alinhamento em structs de erro e contexto.
    - Organizar membros de structs do maior para o menor para reduzir o padding implícito do compilador.
- **Consequência:** Garante que as estruturas caibam em chunks de leitura únicos da CPU, evitando acessos desalinhados que degradam a performance.

### 005: Uso de Libarchive para Backup
- **Contexto:** Necessidade de criar um backup comprimido contendo múltiplos arquivos e diretórios.
- **Decisão:** Utilizar a biblioteca `libarchive` em vez de chamadas de sistema (tar/zip).
- **Consequência:** Permite maior controle sobre o processo de arquivamento, tratamento de erros granular via bitmask e portabilidade sem depender de utilitários externos.

## Próximos Passos
1. Criar a estrutura básica de diretórios do projeto (`src/`, `include/`).
2. Configurar o arquivo `meson.build` inicial.
3. Iniciar o protótipo da leitura de wallpaper via GSettings.
