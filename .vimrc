filetype indent plugin on 

syntax on

set nocompatible

set number cursorline

set incsearch hlsearch

set fileencoding=utf-8 encoding=utf-8

set expandtab softtabstop=2 tabstop=2 shiftround shiftwidth=2

set nobackup

set cc=80
set showmatch
set backspace=indent,eol,start

set laststatus=2
set showmode

set autowrite

set scrolloff=5
set nowrap
set mouse=a

set foldenable
set foldmethod=manual
set foldlevelstart=0

set statusline=%F%m%r\ [EOL=%{&fileformat}]\ [TYPE=%Y]\ [ASCII=%03.3b]\ [POS=%04l,%04v]\ [%p%%]\ [LINES=%L]

if has('win64') || has ('win32')
  let s:build_cmd = "pushd build &&
      \ cmake -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug .. && 
      \ ninja -j %NUMBER_OF_PROCESSORS% &&
      \ popd\<CR>"
else
  let s:build_cmd = "pushd build &&
      \ cmake -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug .. && 
      \ ninja -j $(getconf _NPROCESSORS_ONLN) &&
      \ popd\<CR>"
endif

function! s:Build()
  execute 'cd ' . findfile('CMakeLists.txt', '.;')
  if !isdirectory('build')
    call mkdir('build')
  endif
  let build_tab = bufwinnr('__BUILD__')
  if build_tab == -1
    tabedit __BUILD__ 
    terminal ++curwin
  else
    execute build_tab . 'wincmd w'
  endif
  call term_sendkeys(build_tab, build_cmd)
endfunction

inoremap <C-b> :call Build()<CR>
nnoremap <C-b> :call Build()<CR>
