function(pascal_to_camel input output)
    string(SUBSTRING ${input} 0 1 firstChar)
    string(TOLOWER ${firstChar} firstCharLower)
    string(SUBSTRING ${input} 1 -1 restOfString)
    set(${output} "${firstCharLower}${restOfString}" PARENT_SCOPE)
endfunction()

function(pascal_to_spaced input output)
    set(result "")
    string(LENGTH "${input}" len)
    foreach(i RANGE 0 ${len})
        string(SUBSTRING "${input}" ${i} 1 char)
        if(i GREATER 0)
            string(REGEX MATCH "^[A-Z]" is_upper "${char}")
            if(is_upper)
                set(result "${result} ")
            endif()
        endif()
        set(result "${result}${char}")
    endforeach()
    set(${output} "${result}" PARENT_SCOPE)
endfunction()
