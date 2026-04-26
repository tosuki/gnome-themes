# Project Spec

A primeira versão da aplicação deve ser capaz de identificar e gerenciar as seguintes configurações do GNOME:

### Escopo de Identificação
- [ ] **Wallpaper:** Identificar a imagem definida atualmente.
- [ ] **Tema GTK:** Identificar o tema usado e sua localização física.
- [ ] **Tema de Ícones:** Identificar o conjunto de ícones e sua localização.
- [ ] **Cursor:** Identificar o tema e o tamanho configurado.
- [ ] **GNOME Shell:** Identificar o tema aplicado à interface do Shell.
- [ ] **Fontes:** Capturar as configurações de fontes (Interface, Documentos, Monoespaçada e Títulos).
- [ ] **Preferência de Cor:** Identificar se o sistema está em modo Dark ou Light.
- [ ] **Keybindings:** Identificar e gerenciar atalhos de teclado do sistema e personalizados.
- [ ] **Extensões:** Listar as extensões que estão ativas no momento.

### Gerenciamento de Backup e Restauração
- [ ] **Arquivo de Backup:** Gerar um arquivo comprimido contendo tanto os arquivos de configuração quanto os ativos (temas, ícones, cursores).
- [ ] **Tratamento de Temas Globais:** Caso um tema esteja em diretórios de sistema (`/usr/share/themes` ou `/usr/share/icons`), ele deve ser copiado integralmente para o backup.
- [ ] **Restauração Inteligente:** Ao recuperar uma configuração, se um tema original de sistema não estiver presente na máquina atual, a aplicação deve instalá-lo no diretório do usuário (`~/.themes` ou `~/.local/share/icons`) para evitar a necessidade de privilégios de root e garantir a portabilidade.
- [ ] **Ponto de Restauração:** Criar um backup automático da configuração atual antes de aplicar qualquer alteração vinda de um arquivo recuperado.

### Interface (TUI)
- [ ] Navegação intuitiva via teclado.
- [ ] **Dashboard:** Tela para visualizar a configuração ativa com detalhes de caminhos e versões.
- [ ] **Preview Visual:** Utilizar cores ANSI para representar a paleta do tema atual na interface.
- [ ] **Explorador de Arquivos:** Interface interativa para navegar e selecionar o arquivo de backup para restauração.
- [ ] **Confirmação (Dry Run):** Listar todas as alterações que serão feitas antes de efetivá-las no sistema.

## Requisitos Funcionais e Técnicos

- **Linguagem:** C (Padrão C11 ou superior).
- **Integração GNOME:** Utilizar as bibliotecas `GLib` e `GIO` para manipulação de `GSettings` e `dconf`.
- **Persistência de Contexto:** Todo o progresso, decisões de arquitetura e contexto do projeto devem ser mantidos no arquivo `dev.md`.
- **Integridade do Sistema:** A aplicação não deve apagar arquivos existentes, apenas criar novos ou sobrescrever configurações conforme solicitado.
- **Tratamento de Erros:** Utilizar um sistema de máscaras com operações bitwise para categorizar e acumular erros, permitindo feedbacks precisos e diagnósticos múltiplos em uma única operação.
- **Portabilidade:** Priorizar o uso de caminhos baseados nas especificações XDG.
- **Build System:** Utilizar Meson e Ninja para gerenciamento de compilação e dependências.
