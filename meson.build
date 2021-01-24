project('CHIP-8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmake = import('cmake')

minifb = cmake.subproject('minifb')
libminifb = minifb.dependency('minifb')

executable('CHIP-8',
           'cpu.c',
           'disassemble.c',
           'interpret.c',
           'main.c',
           'ram.c',
           'rom.c',
           install : true,
	         dependencies: [libminifb])
