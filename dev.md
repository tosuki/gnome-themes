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
- [x] Suporte a cores dinâmicas na TUI baseadas no esquema de cores do sistema.

## Decisões de Arquitetura (ADR)

... (omitido para brevidade, mas mantido no arquivo real) ...

### 009: Cores ANSI Dinâmicas via GSettings
- **Contexto:** Necessidade de alinhar a estética da TUI com a preferência de cor do usuário (Dark/Light).
- **Decisão:** Consultar `org.gnome.desktop.interface color-scheme` e redefinir os pares de cores do ncurses em tempo de execução.
- **Consequência:** A interface se adapta visualmente, oferecendo um contraste melhor e uma experiência mais integrada ao sistema.

## Próximos Passos
1. Documentação final e limpeza de avisos de compilação.
2. Testes de integração em diferentes distribuições.
