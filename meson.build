project('mmocli', 'cpp', version : '0.6')

compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'msvc'
  add_project_arguments(['/std:c++latest', '-D_WIN32_WINNT=0x0A00'],
                        language : 'cpp')
else
  pthread = dependency('threads')
  add_project_arguments('-std=c++2a', language : 'cpp')
endif

boost = dependency('boost')

poco = dependency('Poco', method: 'cmake', components: ['Foundation', 'Net',
                                                        'NetSSL'])
gtest = dependency('gtest')

if host_machine.system() == 'linux'
  curses = dependency('curses')
elif host_machine.system() == 'windows'
  pdcm_lib = 'pdcurses'
  pdcm_lib_dir = '../deps/pdcursesmod/lib/wincon'
  pdcm_header_dir = 'deps/pdcursesmod/include/wincon'

  curses = declare_dependency(
      link_args : ['-L' + pdcm_lib_dir, '-l' + pdcm_lib],
      include_directories : include_directories(pdcm_header_dir)
  )
endif

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

ezterm_src = [
  'libezterm/include/ezterm/config.hpp'      ,
  'libezterm/include/ezterm/color.hpp'       ,
  'libezterm/src/ezterm_p.hpp'               , 'libezterm/src/ezterm_p.cpp'    ,
  'libezterm/src/window_impl.hpp'            , 'libezterm/src/window_impl.cpp' ,
  'libezterm/include/ezterm/ezterm.hpp'      , 'libezterm/src/ezterm.cpp'      ,
  'libezterm/include/ezterm/widget.hpp'      , 'libezterm/src/widget.cpp'      ,
  'libezterm/include/ezterm/container.hpp'   , 'libezterm/src/container.cpp'   ,
  'libezterm/include/ezterm/interactive.hpp' , 'libezterm/src/interactive.cpp' ,
  'libezterm/include/ezterm/window.hpp'      , 'libezterm/src/window.cpp'      ,
  'libezterm/include/ezterm/label.hpp'       , 'libezterm/src/label.cpp'       ,
  'libezterm/include/ezterm/field.hpp'       , 'libezterm/src/field.cpp'       ,
  'libezterm/include/ezterm/button.hpp'      , 'libezterm/src/button.cpp'
]

ezterm = library('ezterm',
  ezterm_src,
  dependencies : [curses],
  cpp_args: ['-DPDC_DLL_BUILD', '-DEZTERM_BUILDING_THE_LIB'],
  include_directories: ['libezterm/include/ezterm']
)

#-------------------------------------------------------------------------------

executable('ezterm-test',
  ['ezterm_test.cpp'],
  link_with : [ezterm],
  include_directories: ['libezterm/include']
)

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

mmocli_src = [
  'libmmocli/include/mmocli/config.hpp'      ,
  'libmmocli/include/mmocli/on_exit.hpp'     ,
  'libmmocli/include/mmocli/log.hpp'         , 'libmmocli/src/log.cpp'         ,
  'libmmocli/include/mmocli/tcp_server.hpp'  , 'libmmocli/src/tcp_server.cpp'  ,
  'libmmocli/include/mmocli/tcp_client.hpp'  , 'libmmocli/src/tcp_client.cpp'  ,
  'libmmocli/include/mmocli/game_server.hpp' , 'libmmocli/src/game_server.cpp' ,
  'libmmocli/include/mmocli/server.hpp'      , 'libmmocli/src/server.cpp'
]

mmocli_deps = [boost]

if host_machine.system() == 'linux'
  mmocli_deps += pthread
endif

mmocli = library('mmocli',
  mmocli_src,
  dependencies : mmocli_deps,
  cpp_args : ['-DMMOCLI_BUILDING_THE_LIB'],
  include_directories: ['libmmocli/include/mmocli']
)

#-------------------------------------------------------------------------------

server_deps = [boost]

if host_machine.system() == 'linux'
  server_deps += pthread
endif

executable('mmocli-server',
  ['main_server.cpp'],
  link_with : [mmocli],
  dependencies : server_deps,
  include_directories: ['libmmocli/include']
)

#-------------------------------------------------------------------------------

executable('mmocli-client',
  ['main_client.cpp'],
  link_with : [ezterm],
  include_directories: ['libezterm/include']
)

#-------------------------------------------------------------------------------

tests_tcp_server_normal = executable('tests_tcp_server_normal',
  ['tests_tcp_server_normal.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : [mmocli],
  dependencies : [boost, gtest],
  include_directories: ['libmmocli/include']
)

test('tests_tcp_server_normal', tests_tcp_server_normal,
    priority : 100, suite : 'normal', is_parallel : false)

#-------------------------------------------------------------------------------

tests_tcp_server_robustness = executable('tests_tcp_server_robustness',
  ['tests_tcp_server_robustness.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : [mmocli],
  dependencies : [boost, gtest],
  include_directories: ['libmmocli/include']
)

test('tests_tcp_server_robustness', tests_tcp_server_robustness,
    priority : 0, suite : 'normal', is_parallel : false)

#-------------------------------------------------------------------------------

tests_game_server_normal = executable('tests_game_server_normal',
  ['tests_game_server_normal.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : [mmocli],
  dependencies : [boost, gtest],
  include_directories: ['libmmocli/include']
)

test('tests_game_server_normal', tests_game_server_normal,
    priority : 100, suite : 'robustness', is_parallel : false)

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

poco_mailgun_cpp_args = []

if host_machine.system() != 'linux'
  poco_mailgun_cpp_args += '-DPOCO_NO_AUTOMATIC_LIBS=TRUE'
endif

executable('poco_mailgun',
  ['poco_mailgun.cpp'],
  dependencies : [poco],
  cpp_args : poco_mailgun_cpp_args
)
