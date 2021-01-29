project('CHIP-8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmake = import('cmake')
minifb = cmake.subproject('minifb').dependency('minifb')

incdir = include_directories('include')
subdir('src')

executable('chip',
           sources,
           install : true,
           include_directories : incdir,
           dependencies : [minifb])
