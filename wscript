
top = '.'
out = '.build'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx')

    conf.env.FRAMEWORK = ['OpenGL']
    conf.env.CXXFLAGS = ['--std=c++11',
                         '-Wall',
                         '-Werror',
                         '-O2']
    conf.env.LINKFLAGS = ['-lglfw']
    conf.env.DEFINES = ['GL_SILENCE_DEPRECATION', '__gl_h_', 'GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED']

def build(bld):
    bld.recurse('src')
