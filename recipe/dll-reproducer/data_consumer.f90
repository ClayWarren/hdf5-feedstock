program test_data
  use values, only: shared_value
  if (shared_value /= 17) stop 1
  print *, "Fortran DLL data import passed"
end program
