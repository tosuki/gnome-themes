# Contexto de Desenvolvimento - Gnome Themes Manager

Este arquivo serve como o registro de memória do projeto, conforme definido no `PROJECT_SPEC.md`.

## Estado Atual
- [x] Definição de requisitos e especificações no `PROJECT_SPEC.md`.
- [x] Criação do script de dependências `setup_deps.sh`.
- [x] Configuração do ambiente de build (Meson/Ninja).
- [x] Implementação do módulo de identificação de configurações (GSettings).
- [x] Implementação da localização física de ativos (Temas/Ícones).
- [x] Implementação da lógica de backup (Libarchive + Gzip).
- [x] Implementação da lógica de restauração e extração inteligente.
- [x] Desenvolvimento da interface TUI (Ncurses).

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
- **Consequência:** Permite que a função retorne `ERR_FILESYSTEM | ERR_PERMISSION` e a TUI mostre mensagens específicas para cada bit ativo.

### 004: Otimização de Memória e Alinhamento (Padding)
- **Contexto:** Minimizar ciclos de CPU e otimizar o uso do cache.
- **Decisão:** Aplicar `alignas(16)` e organizar structs do maior para o menor membro.
- **Consequência:** Evita acessos desalinhados e garante que as structs caibam em chunks de leitura únicos da CPU.

### 005: Uso de Libarchive para Backup
- **Contexto:** Necessidade de criar um backup comprimido contendo múltiplos arquivos e diretórios.
- **Decisão:** Utilizar a biblioteca `libarchive` em vez de chamadas de sistema (tar/zip).
- **Consequência:** Controle total sobre o fluxo de dados e tratamento de erros sem dependências externas de CLI.

### 006: Interface TUI com Ncurses
- **Contexto:** Prover navegação intuitiva no terminal sem complexidade de GUI pesada.
- **Decisão:** Utilizar `ncurses` com sistema de cores e captura de teclado raw.
- **Consequência:** Interface leve, rápida e compatível com sessões SSH.

## Próximos Passos
1. Refinar o **Explorador de Arquivos** para permitir navegação real por diretórios.
2. Implementar o **Ponto de Restauração** automático (backup de segurança antes da restauração).
3. Adicionar suporte a cores ANSI dinâmicas na TUI para representar o tema atual.
4. Documentação final e limpeza de avisos de compilação.
