project('PCJsApi-Cli', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3']
  )


exe = executable(
  'PCJsApi-Cli',
  'src/main.cpp',
  'src/cli_command.cpp',
  'src/command.cpp',
  'src/zipper.cpp',
  'src/git/git.cpp',
  'src/cache/cache.cpp',
  install : true,
  dependencies : [
    dependency('libzip', required : true),
    dependency('libcurl', required : true),
    subproject('nlohmann_json').get_variable('nlohmann_json_dep'),
  ]
)
