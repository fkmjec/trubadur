project('ladicka', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

qt6 = import('qt6')
qt_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'Multimedia'])

moc_files = qt6.compile_moc(headers : ['audio_input.hpp', 'displayer.hpp', 'frequency_calculator.hpp'],
                            extra_args: ['-DMAKES_MY_MOC_HEADER_COMPILE'],
                            dependencies: qt_dep)

executable('ladicka',
           'main.cpp',
           'displayer.cpp',
           'audio_input.cpp',
           'frequency_calculator.cpp',
           moc_files,
           install : true,
           dependencies: [qt_dep])
