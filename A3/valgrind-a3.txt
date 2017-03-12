 valgrind --leak-check=yes ./a.out
==4437== Memcheck, a memory error detector
==4437== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==4437== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==4437== Command: ./a.out
==4437== 
==4437== Invalid free() / delete / delete[] / realloc()
==4437==    at 0x4C2F24B: operator delete(void*) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4437==    by 0x401F8C: std::default_delete<ship>::operator()(ship*) const (unique_ptr.h:76)
==4437==    by 0x4019A8: std::unique_ptr<ship, std::default_delete<ship> >::~unique_ptr() (unique_ptr.h:236)
==4437==    by 0x4026DC: void std::_Destroy<std::unique_ptr<ship, std::default_delete<ship> > >(std::unique_ptr<ship, std::default_delete<ship> >*) (stl_construct.h:93)
==4437==    by 0x40257A: void std::_Destroy_aux<false>::__destroy<std::unique_ptr<ship, std::default_delete<ship> >*>(std::unique_ptr<ship, std::default_delete<ship> >*, std::unique_ptr<ship, std::default_delete<ship> >*) (stl_construct.h:103)
==4437==    by 0x402269: void std::_Destroy<std::unique_ptr<ship, std::default_delete<ship> >*>(std::unique_ptr<ship, std::default_delete<ship> >*, std::unique_ptr<ship, std::default_delete<ship> >*) (stl_construct.h:126)
==4437==    by 0x401BAE: void std::_Destroy<std::unique_ptr<ship, std::default_delete<ship> >*, std::unique_ptr<ship, std::default_delete<ship> > >(std::unique_ptr<ship, std::default_delete<ship> >*, std::unique_ptr<ship, std::default_delete<ship> >*, std::allocator<std::unique_ptr<ship, std::default_delete<ship> > >&) (stl_construct.h:151)
==4437==    by 0x4017D8: std::vector<std::unique_ptr<ship, std::default_delete<ship> >, std::allocator<std::unique_ptr<ship, std::default_delete<ship> > > >::~vector() (stl_vector.h:424)
==4437==    by 0x401357: main (Assignment3.cxx:229)
==4437==  Address 0x5ab5ca0 is 32 bytes inside a block of size 72 alloc'd
==4437==    at 0x4C2E0EF: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4437==    by 0x401853: std::unique_ptr<ship, std::default_delete<ship> > create_ship<cargo_ship<int>, char const (&) [15]>(char const (&) [15]) (Assignment3.cxx:218)
==4437==    by 0x401327: main (Assignment3.cxx:230)
==4437== 
==4437== 
==4437== HEAP SUMMARY:
==4437==     in use at exit: 72,776 bytes in 2 blocks
==4437==   total heap usage: 3 allocs, 2 frees, 72,784 bytes allocated
==4437== 
==4437== 72 bytes in 1 blocks are definitely lost in loss record 1 of 2
==4437==    at 0x4C2E0EF: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4437==    by 0x401853: std::unique_ptr<ship, std::default_delete<ship> > create_ship<cargo_ship<int>, char const (&) [15]>(char const (&) [15]) (Assignment3.cxx:218)
==4437==    by 0x401327: main (Assignment3.cxx:230)
==4437== 
==4437== LEAK SUMMARY:
==4437==    definitely lost: 72 bytes in 1 blocks
==4437==    indirectly lost: 0 bytes in 0 blocks
==4437==      possibly lost: 0 bytes in 0 blocks
==4437==    still reachable: 72,704 bytes in 1 blocks
==4437==         suppressed: 0 bytes in 0 blocks
==4437== Reachable blocks (those to which a pointer was found) are not shown.
==4437== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==4437== 
==4437== For counts of detected and suppressed errors, rerun with: -v
==4437== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
