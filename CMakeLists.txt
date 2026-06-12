# CMakeList.txt: Üst düzey CMake proje dosyası, genel yapılandırma gerçekleştirin
# ve alt projeleri buraya ekleyin.
#
cmake_minimum_required (VERSION 3.10)

# Destekleniyorsa MSVC derleyicileri için Çalışırken Yeniden Yükleme aracını etkinleştirin.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("cangine")

# Alt projeleri ekleyin.
add_subdirectory ("cangine")

option(BUILD_EXAMPLES "Build examples." OFF)
if(BUILD_EXAMPLES)
    add_subdirectory("examples/audio")
    add_subdirectory("examples/sandbox")
    add_subdirectory("examples/helloworld")
    add_subdirectory("examples/simplehelloworld")
    add_subdirectory("examples/camera")
    add_subdirectory("examples/panel")
    add_subdirectory("examples/ui")
    add_subdirectory("examples/inputfield")
    add_subdirectory("examples/aimtrainer")
    add_subdirectory("examples/survival")
endif()
