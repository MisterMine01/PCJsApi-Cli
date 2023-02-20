project(
  'PCJsApi-Cli',
  'cpp',
  version : '0.1',
  default_options : ['warning_level=3']
)


exe = executable(
  'PCJsApi-Cli',
  'src/main.cpp',
  'src/git/git.cpp',
  'src/cache/cache.cpp',
  install : true,
  install_dir : meson.current_source_dir() + '/bin',
  dependencies : [
    meson.get_compiler('cpp').find_library('stdc++fs'),
    dependency('libzip', required : true),
    dependency('libcurl', required : true),
    subproject('nlohmann_json').get_variable('nlohmann_json_dep'),
    subproject('mm1_simpcli').get_variable('simp_cli_dep'),
  ]
)