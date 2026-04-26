# GNOME Themes Manager (GTM)

O **GNOME Themes Manager** é uma ferramenta de linha de comando com interface TUI, desenvolvida em C, projetada para facilitar o backup e a restauração completa da aparência do ambiente GNOME.

Ao contrário de outras ferramentas que salvam apenas nomes de configurações, o GTM empacota os ativos reais (temas, ícones, cursores e wallpapers), garantindo que sua personalização seja portátil entre diferentes máquinas e instalações.

## ✨ Funcionalidades

- **Backup Completo:** Identifica e comprime Wallpaper, Tema GTK, Ícones, Cursor, Fontes, Keybindings e Esquema de Cores.
- **Restauração Inteligente:** Detecta se um tema original era global e, caso não exista no destino, instala-o localmente para o usuário (`~/.themes` ou `~/.local/share/icons`).
- **Ponto de Restauração Automático:** Cria um backup de segurança da configuração atual antes de aplicar qualquer restauração.
- **Interface TUI:** Navegação intuitiva via teclado utilizando `ncurses`.
- **Tratamento de Erros Granular:** Sistema baseado em máscaras bitwise para diagnósticos precisos.

## 🛠️ Detalhes Técnicos (Contribuição)

O projeto foi construído com foco em performance e robustez:

- **Sistema de Erros Bitwise:** Utilizamos um `enum` onde cada bit representa uma falha. Isso permite que uma única operação reporte múltiplos problemas simultâneos (ex: erro de leitura + permissão negada).
- **Alinhamento de Memória (Padding):** As estruturas de dados críticas utilizam `alignas(16)` e organização de membros do maior para o menor para otimizar o acesso via cache da CPU e evitar chunks de leitura duplicados.
- **GLib/GIO:** Integração profunda com as APIs oficiais do GNOME para manipulação de `GSettings`.
- **Libarchive:** Manipulação programática de arquivos `.tar.gz` para backups eficientes.

## 🚀 Instalação e Compilação

### 1. Dependências
O projeto fornece um script automatizado para instalar as dependências no Fedora, Ubuntu/Debian e Arch Linux:

```bash
chmod +x setup_deps.sh
./setup_deps.sh
```

### 2. Compilação
Utilizamos o sistema de build **Meson**:

```bash
meson setup build
meson compile -C build
```

### 3. Execução
```bash
./build/gtm
```

## 🤝 Como Contribuir

1. Leia o arquivo `dev.md` para entender o contexto atual e as Decisões de Arquitetura (ADRs) já tomadas.
2. Siga o padrão de commits: `feat:`, `fix:`, `adjust:`, `refact:`, `docs:`, `chore:`.
3. Certifique-se de que novas structs sigam os padrões de alinhamento de memória definidos na ADR 004.
4. Sempre documente novas funções no `include/`.

## 📄 Licença
Este projeto é distribuído sob a licença MIT.
