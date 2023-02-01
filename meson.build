project('ladicka', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

gtkmm_dep = dependency('gtkmm-4.0', version: '>= 4.6.0')

executable('ladicka',
           'main.cpp',
           'displayer.cpp',
           install : true,
           dependencies: gtkmm_dep)
