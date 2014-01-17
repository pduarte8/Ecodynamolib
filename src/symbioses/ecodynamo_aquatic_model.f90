!
! ECODYN Fortran to C interface
!
! The routinese are to be used from C/C++ to access ECODYN functionality
!
module ecodynamo_aquatic_model
    use iso_c_binding
    implicit none
    real(4), dimension(3), save, target :: ctrl_vol = (/1.0, 1.0, 1.0/)
contains
    function ecodynamo_initialize() result(istat) bind(c)
        integer(C_INT) :: istat
        istat = 0
    end function

    function ecodynamo_finalize() result(istat) bind(c)
        integer(C_INT) :: istat
        istat = symb4km_finalize()
    end function

    function ecodynamo_get_norganisms() result(lval) bind(c)
        integer(C_INT) :: lval
        lval = 6
    end function

    function ecodynamo_get_nbb() result(lval) bind(c)
        integer(C_INT) :: lval
        lval = nbb_cop
    end function

    subroutine ecodynamo_control_volume(vol) bind(c)
        type(C_PTR) :: vol
        real(4), dimension(:), pointer :: fval
        integer :: i
        call c_f_pointer(vol, fval, (/3/))
        fval(1) = ctrl_vol(1)
        fval(2) = ctrl_vol(2)
        fval(3) = ctrl_vol(3)
    end subroutine

    subroutine ecodynamo_get_organism(o, n, lat, lon, depth) bind(c)
        type(C_PTR), value :: o
        integer(C_INT), value :: n
        real(C_FLOAT), intent(in), value :: lat, lon, depth
        integer :: i, j, m
        real(4), dimension(1) :: ii, jj, la, lo
        type(organism_t) :: bob ! a copepod named bob

        bob = organism_init_object(o)
        call bob%set_num_body_burdens(1)

        call bob%set_fat_content(1.0)
        call bob%set_weight(1.0)
        call bob%set_abundance(1.0)

        call bob%set_feeding_rate(-1.0)
        call bob%set_growth_rate(-1.0)
        call bob%set_mortality_rate(-1.0)
        call bob%set_reproduction_rate(-1.0)
    end subroutine

end module
