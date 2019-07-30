set nocompatible              " be iMproved, required
filetype off                  " required

" Set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

" Let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'rhysd/vim-clang-format'
Plugin 'SirVer/ultisnips'
Plugin 'honza/vim-snippets'
Plugin 'vim-scripts/bash-support.vim'
Plugin 'vim-scripts/bats.vim'
Plugin 'vim-syntastic/syntastic'
Plugin 'kawaz/batscheck.vim'
Plugin 'martinda/Jenkinsfile-vim-syntax'
Plugin 'tomtom/tcomment_vim'
Plugin 'machakann/vim-highlightedyank'
Plugin 'itchyny/lightline.vim'
Plugin 'majutsushi/tagbar'
Plugin 'ctrlpvim/ctrlp.vim'
Plugin 'rdnetto/YCM-Generator'
Plugin 'tpope/vim-fugitive'
Plugin 'skywind3000/asyncrun.vim'

" All of Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line

set exrc
set secure

set tabstop=8
set softtabstop=0
set expandtab
set shiftwidth=4
set smarttab

:imap jk <Esc>

:colorscheme torte

let g:netrw_banner = 0

" Set this shortcuts to easily navigate through panes
nnoremap <c-j> <c-w><c-j>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

" Save file on Ctrl-s
:nmap <C-s> :w<CR>
:imap <C-s> <Esc>:w<CR>

" Copy and paste on Ctrl-c and Ctrl-v
:vnoremap <C-c> "+y 
:inoremap <C-v> <C-o>"+p

" Map Ctrl-E for file explorer
:map <C-e> :Explore<CR>

" File explorer customization
let g:netrw_banner = 0
let g:netrw_liststyle = 3

" Tab navigation like Firefox.
nnoremap <C-t> :tabnew<CR>:Explore<CR>
inoremap <C-t> <Esc>:tabnew<CR>:Explore<CR>

" Show me line numbers for better navigataion and debugging
set number
" Set its width
set numberwidth=2
" Set colors for the bar
set t_Co=256
highlight LineNr term=bold cterm=NONE ctermfg=lightyellow ctermbg=darkgrey gui=NONE guifg=DarkGrey guibg=NONE

" Add vertical rule
set colorcolumn=120
highlight ColorColumn ctermbg=4

" UltiSnips shortcuts config
let g:UltiSnipsExpandTrigger="<c-k>"
let g:UltiSnipsJumpForwardTrigger="<c-m>"
let g:UltiSnipsJumpBackwardTrigger="<c-b>"
let g:UltiSnipsListSnippets="<c-n>"

let g:BASH_Ctrl_j="<c-p>"

" Set this shortcuts to easily navigate through panes
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

" Syntastic configuration
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*
let g:syntastic_always_populate_loc_list = 0
let g:syntastic_auto_loc_list = 0
let g:syntastic_check_on_open = 0
let g:syntastic_check_on_wq = 0

let g:ycm_extra_conf_globlist = ['~/.vim/*','~/Workspace/AgriWatch/*','~/Workspace/Dynks*','~/Workspace/Blinker*','~/Workspace/Therm*','~/Workspace/Magnetoblinker*','~/Workspace/Beeblinker*']
let g:ycm_max_diagnostics_to_display = 1000
let g:ycm_complete_in_comments = 1
let g:ycm_complete_in_strings = 1
let g:ycm_collect_identifiers_from_comments_and_strings = 1
let g:ycm_seed_identifiers_with_syntax = 1
let g:ycm_add_preview_to_completeopt = 1
let g:ycm_show_diagnostics_ui = 1
let g:ycm_disable_for_files_larger_than_kb = 0
let g:ycm_collect_identifiers_from_tags_files = 1
let g:ycm_always_populate_location_list = 1

:nmap <c-i> :YcmCompleter GoToInclude<CR>
:nmap <F3> :YcmCompleter GoToDefinition<CR>
:nmap <c-d> :YcmCompleter GoToDeclaration<CR>

" Highlight yanked region with timeout
let g:highlightedyank_highlight_duration = 2000
hi HighlightedyankRegion cterm=reverse gui=reverse ctermfg=187

" Highlight search result
:set hlsearch
hi Search ctermbg=lightblue

" Show filename in statusline
:set statusline=%f

" Show character count in visual mode
:set showcmd

" For the lightline plugin
set laststatus=2
let g:lightline = {
      \ 'colorscheme': 'powerline',
      \ 'active': {
      \   'left': [ [ 'mode', 'paste' ],
      \             [ 'gitbranch', 'readonly', 'filename', 'modified', 'absolutepath' ] ]
      \ },
      \ 'component_function': {
      \   'gitbranch': 'fugitive#head'
      \ },
      \ }

" Map toggle Tagbar
nmap <F8> :TagbarToggle<CR>

" AsyncRun configuration
:let g:asyncrun_open = 14
:let g:asyncrun_bell = 10
nmap <F10> :AsyncRun 

" =====================================================================================================================
" MACROS FOR C PROGRAMMING
" =====================================================================================================================

" Macros for fast unit tests in C handling
let @q = 'V}ky/EXECUT}kkp}kklllllllllllcRUN_TEST(jkV}kk:s/()/)/g/DECLARATION OF THE TEST CASESjjVjjjy/DEFINITION OF THE TESTjpV}:s/;/{}\r/g:ClangFormat'
let @g = 'V}k"ky/EXECUTION}kk"kp}kktUcRUN_TEST(jkV}:s/()/)/g/DEFINITION OF PRIVATE FUNkk"kPV}:s/;/{}\r/g:ClangFormat:noh'

" Macro which makes definitions of C functions from declarations
let @b = 'V}:s/;/{}\r/g:ClangFormat'
