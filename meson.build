project('ladicka', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

qt5_dep = dependency('qt5', modules: ['Core', 'Gui', 'Widgets'])

executable('ladicka',
           'main.cpp',
           'displayer.cpp',
           install : true,
           dependencies: [qt5_dep])
