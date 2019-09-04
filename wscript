import sys

top = '.'
out = '.build'

tools = 'compiler_cxx compiler_c'

def options(opt):
    opt.load(tools)

def configure(conf):
    conf.load(tools)

    conf.env.CXXFLAGS = [
        '--std=c++11',
        '-Wall',
        '-Werror',
        '-O2',
        '-Wno-int-to-pointer-cast',
    ]

    conf.env.LINKFLAGS = [
        '-lglfw',
    ]

    if sys.platform == 'darwin':
        conf.env.FRAMEWORK = ['OpenGL']
        conf.env.DEFINES = [
            'GL_SILENCE_DEPRECATION',
            # '__gl_h_',
            # 'GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED',
        ]

    elif sys.platform == 'linux':
        conf.env.LINKFLAGS.extend([
            '-lGL',
            '-lGLEW',
            '-ldl',
        ])

def build(bld):
    bld.stlib(
        features='c',
        source='vendor/glad/src/glad.c',
        includes=['vendor/glad/include'],
        export_includes=['vendor/glad/include'],
        target='glad',
    )

    bld(
        includes=['vendor/glm/glm'],
        export_includes=['vendor/glm'],
        name='glm',
    )

    bld.stlib(
        source='vendor/stb/stb_image.cpp',
        includes=['vendor/stb'],
        export_includes=['vendor'],
        target='stb',
    )

    bld.recurse('src')
