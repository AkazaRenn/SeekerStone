function(lower_initial_char input output)
    string(SUBSTRING ${input} 0 1 firstChar)
    string(TOLOWER ${firstChar} firstCharLower)
    string(SUBSTRING ${input} 1 -1 restOfString)
    set(${output} "${firstCharLower}${restOfString}" PARENT_SCOPE)
endfunction()

