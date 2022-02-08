from conans import ConanFile, CMake, tools

class AlyaRecipe(ConanFile):

    name = 'alya'
    version = '0.0.0.2'
    settings = "os", "compiler", "build_type", "arch"
    options = {}
    default_options = {}

    exports_sources = "CMakeLists.txt", "src/*", "*.hpp", "test_apps/*"
    requires = ['boost/1.78.0', 'glm/0.9.9.8', 'stb/cci.20210713', 'utfcpp/3.2.1', 'libsndfile/1.0.31', 'assimp/5.0.1']
    
    generators = 'cmake', 'cmake_find_package'

    def _configure(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        self._configure().build()

    def package(self):
        self.copy(pattern='*.dll', dst='bin', keep_path=False)
        self.copy(pattern='*.lib', dst='lib', keep_path=False)
        self.copy(pattern="*.hpp", dst='', keep_path=True)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.includedirs = ['include']
    def imports(self):
        self.copy(pattern="*.dll", dst="")
