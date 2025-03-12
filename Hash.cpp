////////////////////////////////////////////////////////////////
# include "Hash.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
std::random_device                                    Hash::dev;
std::uniform_int_distribution <u64>     Hash::dist( 0, U64MAX );
std::mt19937_64                                       Hash::gen;
u64                     Hash::_sq[ Board::SIZ ][ EMPTY ][ RED ];
u64                                           Hash::_the_switch;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Hash::initialize( u64 seed )
{
    gen.seed( seed );
    _the_switch = genu64();
    for( ofst_t i = 0; i < Board::SIZ; ++i ){
        for( fig_t j = 0; j < EMPTY; ++j ){
            _sq[ i ][ j ][ BLACK ] = genu64();
            _sq[ i ][ j ][ WHITE ] = genu64();
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// <> 2.1.19. If you p=r==e=s=s a ballpoint pen with your finger, 
// resting on a hard surf-ace, while tilting it, then as long as 
// the pen forms a s-m-a-l-l angle with the perpendicular to the 
// surface, it will obediently f-o-l-low your finger. As soon as 
// the angle of inclination of the pen exceeds a certain maximum 
// value a_max, it will slip out f=r=o=m u=n=der your finger, no 
// matter how hard or softly you press on it. E=x=p=e=r=i=m=e=nt 
// yourself and estimate the coefficient of friction between the 
// ball of the pen and the surface on which it rests. 
// ####  |              ########################################
// ####  |       // F   ### "...it will obediently follow your
// ####  |      ///     ########################## finger..." ##
// ####  |     ///      # here means; the end of the pen where #
// ####  |    ///       ## it meats the surface will stay still 
// ####  | a / /        ########################################
// ####  |  / /         ## and the upper end will follow your ##
// ####  | / /          ### finger #############################
// ####  | /            ########################################
// ------+------------------------------------------------------
// #############################################################
// #############################################################
////////////////////////////////////////////////////////////////
// The h-o-r-i-z-o-n-t-a-l a=n=d vertical components o_f F a=r=e: 
// F(x) = Fsin(a) and F(y) = Fcos(a), F(y) will cause a reaction
// of the plane N equal to it, F(x) will increase i_f a increase 
// so the lower end wil=l stay motionless until F(x) reaches the
// critical value _N, where _ is the coefficient of friction, so
// F(x) = _F(y), Fsin(a) = _Fcos(a), and _ = tg(a)
////////////////////////////////////////////////////////////////
