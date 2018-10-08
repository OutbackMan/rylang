filetype indent plugin on 

syntax on

set nocompatible

set number

set incsearch hlsearch 
set fileencoding=utf-8 encoding=utf-8

set expandtab softtabstop=2 tabstop=2 shiftround shiftwidth=2

set nobackup

set cc=80
set showmatch
set backspace=indent,eol,start

set laststatus=2
set showmode

set autoindent

set autowrite

set scrolloff=5
set nowrap
set mouse=a

set foldenable
set foldmethod=manual
set foldlevelstart=0

if !exists('g:os')
  if has('win64') || has ('win32')
    let g:os = 'Windows'
  else
    let g:os = substitute(system('uname'), '\n', '', '')
  endif
endif

if !exists('g:build_cmd')
if g:os == 'Windows'
  let g:build_cmd = 'pushd build &&
      \ cmake -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug .. &&
      \ ninja -j ' . $NUMBER_OF_PROCESSORS . "\<CR>"
else
  let g:build_cmd = 'pushd build &&
      \ cmake -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug .. &&
      \ ninja -j ' . system('getconf _NPROCESSORS_ONLN') . "\<CR>"
endif
endif

let g:build_buf = 0
let g:term_build_buf = 0

function! EnterBuild()
  if findfile("CMakeLists.txt") == "CMakeLists.txt"
    if !isdirectory('build')
      call mkdir('build')
    endif
    tabedit __BUILD__
    let g:build_buf = bufnr('%')
    terminal ++curwin
    let g:term_build_buf = bufnr('%')
    call term_sendkeys(g:term_build_buf, g:build_cmd)
  else
    echoerr 'Please perform build at project root directory'
  endif
endfunction

inoremap <C-b> :call EnterBuild()<CR>
nnoremap <C-b> :call EnterBuild()<CR>

tnoremap <C-e> exit<CR>

autocmd BufEnter * if g:build_buf != 0 | execute 'bdelete ' . g:build_buf | let g:build_buf = 0 | endif
