" Vim syntax file
" Language:         DISML (Do It Simple Markup Language)
" Maintainer:       Francesco Massei <fmasse@gmail.com>
" Latest Revision:  2010-06-11

if exists("b:current_syntax")
    finish
endif

let s:cpo_save = &cpo
set cpo&vim

syn keyword dismlTodo           contained TODO FIXME XXX NOTE
syn region dismlComment         display oneline start='\%(^\|\s\)#' end='$'
                                \ contains=dismlTodo,@Spell
syn region dismlString          start=+"+ skip=+\\["\n]+ end=+"+
                                \ contains=dismlEscape
syn match dismlEscape           contained display +\\\\"+
syn match dismlObject           '\w*$'
syn region dismlDocumentHeader  start='^DISML' end='$'

hi def link dismlTodo           Todo
hi def link dismlComment        Comment
hi def link dismlDocumentHeader PreProc
hi def link dismlString         String
hi def link dismlObject         Type

let b:current_syntax = "disml"

let &cpo = s:cpo_save
unlet s:cpo_save
