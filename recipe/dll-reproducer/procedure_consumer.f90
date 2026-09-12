program test_procedure
  use values, only: get_value
  if (get_value() /= 17) stop 1
  print *, "Fortran DLL procedure import passed"
end program
