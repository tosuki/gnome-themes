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
- [x] Implementação do gerenciamento de Keybindings (Sistema e Personalizados).
- [x] Refinamento do Explorador de Arquivos interativo.

## Decisões de Arquitetura (ADR)

### 001: Uso de GLib e GIO
... (omitido para brevidade, mas mantido no arquivo real) ...

### 008: Explorador de Arquivos Baseado em Dirent e Ncurses
- **Contexto:** Necessidade de selecionar backups de forma visual sem digitar caminhos manuais.
- **Decisão:** Implementar um loop de navegação que lê `opendir()` e filtra por diretórios ou sufixo `.tar.gz`.
- **Consequência:** Melhora significativa na UX, permitindo navegação profunda no sistema de arquivos para localizar backups.

## Próximos Passos
1. Adicionar suporte a cores ANSI dinâmicas na TUI para representar o tema atual.
2. Documentação final e limpeza de avisos de compilação.
3. Testes de integração em diferentes distribuições.
