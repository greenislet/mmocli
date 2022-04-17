project('mmocli', 'cpp', version : '0.4')

cpp = meson.get_compiler('cpp')

deps = []

if cpp.get_id() == 'msvc'
  add_project_arguments(['/std:c++latest', '-D_WIN32_WINNT=0x0A00'], language : 'cpp')
else
  pthread = dependency('threads')
  deps += pthread
  add_project_arguments('-std=c++2a', language : 'cpp')
endif

boost = dependency('boost')

deps += boost

gtest = dependency('gtest')

src = [
  'config.hpp',
  'log.hpp',
  'log.cpp',
  'on_exit.hpp',
  'tcp_server.hpp',
  'tcp_server.cpp',
  'tcp_client.hpp',
  'tcp_client.cpp',
  'game_server.hpp',
  'game_server.cpp',
  'server.cpp',
  'server.hpp',
]

mmocli = library('mmocli', src,
  dependencies : deps,
  cpp_args : '-DMMOCLI_BUILDING_THE_LIB'
)

executable('mmocli-server', ['main_server.cpp'],
  link_with : mmocli,
  dependencies : deps,
)

deps += gtest

tests_tcp_server_normal = executable('tests_tcp_server_normal',
  ['tests_tcp_server_normal.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : mmocli,
  dependencies : deps,
)

tests_tcp_server_robustness = executable('tests_tcp_server_robustness',
  ['tests_tcp_server_robustness.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : mmocli,
  dependencies : deps,
)

tests_game_server_normal = executable('tests_game_server_normal',
  ['tests_game_server_normal.cpp', 'tests.cpp', 'tests.hpp'],
  link_with : mmocli,
  dependencies : deps,
)

test('tests_tcp_server_normal', tests_tcp_server_normal,
    priority : 100, suite : 'normal', is_parallel : false)
test('tests_game_server_normal', tests_game_server_normal,
    priority : 90, suite : 'normal', is_parallel : false)

test('tests_tcp_server_robustness', tests_tcp_server_robustness,
    priority : 0, suite : 'robustness', is_parallel : false)
