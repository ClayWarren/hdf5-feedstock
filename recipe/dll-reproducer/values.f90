module values
  integer :: shared_value = 17
contains
  integer function get_value()
    get_value = shared_value
  end function
end module
