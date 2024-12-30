cmake_minimum_required(VERSION 3.5)
project(
  embed
  VERSION 1.0.0
  LANGUAGES C
)

add_executable(embed embed.c)

function(embed_files target_name)
  set(out_file_dir "${CMAKE_CURRENT_BINARY_DIR}/embed/${target_name}")
  set(out_file_source "${out_file_dir}/${target_name}.c")
  set(out_file_header "${out_file_dir}/${target_name}.h")
  add_custom_command(
    OUTPUT ${out_file_source} ${out_file_header}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${out_file_dir}"
    COMMAND embed "${out_file_source}" "${target_name}" ${ARGN}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${out_file_source}
        ${out_file_header}
    DEPENDS embed ${ARGN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building binary file for embedding ${ARGN} into ${out_file_source}"
    VERBATIM)

  add_library(${target_name} STATIC ${out_file_source})
  add_compile_definitions(${target_name} PRIVATE EMBED_IMPL)
  target_include_directories(${target_name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/embed/${target_name}")
endfunction()
