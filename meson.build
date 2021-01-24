project('CHIP-8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmake = import('cmake')

minifb = cmake.subproject('minifb')
libminifb = minifb.dependency('minifb')

executable('chip',
           'console.c',
           'disassemble.c',
           'interpret.c',
           'main.c',
           install : true,
	         dependencies : [libminifb])
