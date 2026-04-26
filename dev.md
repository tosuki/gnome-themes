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
- [ ] Planejamento e Scaffolding da interface gráfica GTK4/Libadwaita.

## Decisões de Arquitetura (ADR)

... (omitido para brevidade, mas mantido no arquivo real) ...

### 010: Transição para GTK4 e Libadwaita
- **Contexto:** Necessidade de uma interface gráfica moderna que siga as diretrizes do GNOME (HIG).
- **Decisão:** Utilizar GTK4 como toolkit e Libadwaita para componentes de UI específicos do GNOME.
- **Consequência:** A aplicação terá um visual "oficial" do GNOME, suporte nativo a temas (claro/escuro) e será adaptável para diferentes tamanhos de tela. O motor lógico (backup/restauração) permanecerá desacoplado da interface.

## Próximos Passos
1. Atualizar o `setup_deps.sh` para incluir `libadwaita-devel`.
2. Criar o protótipo da janela principal em GTK4.
3. Implementar o sistema de "Dual Interface" (TUI e GUI coexistindo no mesmo binário ou binários separados).
4. Integrar os sinais do GTK com a lógica de backup/restauração já existente.
