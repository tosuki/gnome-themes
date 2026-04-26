project('gnome-themes-manager', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c11'])

# Dependências
glib_dep = dependency('glib-2.0')
gio_dep = dependency('gio-2.0')
archive_dep = dependency('libarchive')

# Incluir diretórios
inc = include_directories('include')

# Arquivos fonte
sources = [
  'src/main.c',
  'src/gnome_config.c'
]

# Executável
executable('gtm',
  sources,
  include_directories : inc,
  dependencies : [glib_dep, gio_dep, archive_dep],
  install : true
)
