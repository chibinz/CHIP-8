project('CHIP-8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmake = import('cmake')
minifb = cmake.subproject('minifb').dependency('minifb')

incdir = include_directories('include')
subdir('src')

libchip = static_library('chip',
                         lib_src,
                         include_directories : incdir)
executable('chip',
           exe_src,
           link_with : libchip,
           include_directories : incdir,
           dependencies : [minifb])
