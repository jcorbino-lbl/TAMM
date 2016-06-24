//kbn p2
//     i0 ( p2 h1 )_xf + = -1 * Sum ( h6 ) * x ( p2 h6 )_x * i1 ( h6 h1 )_f
//         i1 ( h6 h1 )_f + = 1 * f ( h6 h1 )_f
//         i1 ( h6 h1 )_ft + = 1 * Sum ( p7 ) * t ( p7 h1 )_t * i2 ( h6 p7 )_f
//             i2 ( h6 p7 )_f + = 1 * f ( h6 p7 )_f
//             i2 ( h6 p7 )_vt + = 1 * Sum ( h5 p4 ) * t ( p4 h5 )_t * v ( h5 h6 p4 p7 )_v
//         i1 ( h6 h1 )_vt + = -1 * Sum ( h4 p3 ) * t ( p3 h4 )_t * v ( h4 h6 h1 p3 )_v
//         i1 ( h6 h1 )_vt + = -1/2 * Sum ( h5 p3 p4 ) * t ( p3 p4 h1 h5 )_t * v ( h5 h6 p3 p4 )_v
//ckbn p2
//     i0 ( p2 h1 )_xf + = 1 * Sum ( p7 h6 ) * x ( p2 p7 h1 h6 )_x * i1 ( h6 p7 )_f
//         i1 ( h6 p7 )_f + = 1 * f ( h6 p7 )_f
//         i1 ( h6 p7 )_vt + = 1 * Sum ( h4 p3 ) * t ( p3 h4 )_t * v ( h4 h6 p3 p7 )_v
//ckbn p2
//     i0 ( p2 h1 )_xv + = -1/2 * Sum ( p7 h6 h8 ) * x ( p2 p7 h6 h8 )_x * i1 ( h6 h8 h1 p7 )_v
//         i1 ( h6 h8 h1 p7 )_v + = 1 * v ( h6 h8 h1 p7 )_v
//         i1 ( h6 h8 h1 p7 )_vt + = 1 * Sum ( p3 ) * t ( p3 h1 )_t * v ( h6 h8 p3 p7 )_v

#include "variables.h"
#include <iostream>
#include "tensor.h"
#include "t_mult.h"
#include "t_assign.h"
#include "expression.h"

namespace ctce {

  static Assignment a_x1_1_1, a_x1_1_2_1, a_x1_2_1, a_x1_3_1;
  static Multiplication m_x1_1_2_2, m_x1_1_2, m_x1_1_3, m_x1_1_4, m_x1_1, m_x1_2_2, m_x1_2, m_x1_3_2, m_x1_3;
  static Tensor x1_1_1, x1_1_2_1, x1_2_1, x1_3_1;

  extern "C" {
    void gen_expr_ipccsd_x1_cxx_() {
      
      static bool set_ipccsd = true;
      Tensor tC, tA, tB;
      
      DistType idist = (Variables::intorb()) ? dist_nwi : dist_nw;

      if (set_ipccsd) {
	set_ipccsd = false;

	//i1 ( h6 h1 )_f + = 1 * f ( h6 h1 )_f
        tC = Tensor2(H6B,H1B,0,1,iV_tensor, dist_nw, dim_ov);
        tA = Tensor2(H6B,H1B,0,1,F_tensor, dist_nw, dim_n);
	a_x1_1_1 = Assignment(tC,tA,1.0,ivec(H6B,H1B),ivec(H6B,H1B));
	
	//i2 ( h6 p7 )_f + = 1 * f ( h6 p7 )_f
        tC = Tensor2(H6B,P7B,0,1,iV_tensor, dist_nw, dim_ov);
        tA = Tensor2(H6B,P7B,0,1,V_tensor, dist_nw, dim_n);
	a_x1_1_2_1 = Assignment(tC,tA,1.0,ivec(H6B,P7B),ivec(H6B,P7B));

	//i2 ( h6 p7 )_vt + = 1 * Sum ( h5 p4 ) * t ( p4 h5 )_t * v ( h5 h6 p4 p7 )_v
        tC = Tensor2(H6B,P7B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P4B,H5B,0,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H5B,H6B,P4B,P7B,0,0,1,1,iV_tensor, idist, dim_n);
	m_x1_1_2_2 = Multiplication(tC,tA,tB,1.0);

	//i1 ( h6 h1 )_ft + = 1 * Sum ( p7 ) * t ( p7 h1 )_t * i2 ( h6 p7 )_f
        tC = Tensor2(H6B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P7B,H1B,0,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor2(H6B,P7B,0,1,iV_tensor, dist_nw, dim_ov);
	m_x1_1_2 = Multiplication(tC,tA,tB,1.0);

	//i1 ( h6 h1 )_vt + = -1 * Sum ( h4 p3 ) * t ( p3 h4 )_t * v ( h4 h6 h1 p3 )_v
        tC = Tensor2(H6B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P3B,H4B,0,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H4B,H6B,H1B,P3B,0,0,1,2,iV_tensor, idist, dim_n);
	m_x1_1_3 = Multiplication(tC,tA,tB,-1.0);

	//i1 ( h6 h1 )_vt + = -1/2 * Sum ( h5 p3 p4 ) * t ( p3 p4 h1 h5 )_t * v ( h5 h6 p3 p4 )_v
        tC = Tensor2(H6B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor4(P3B,P4B,H1B,H5B,0,0,1,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H5B,H6B,P3B,P4B,0,0,1,1,iV_tensor, idist, dim_n);
	m_x1_1_4 = Multiplication(tC,tA,tB,-0.5);

#if 0
	//i0 ( p2 h1 )_xf + = -1 * Sum ( h6 ) * x ( p2 h6 )_x * i1 ( h6 h1 )_f
        tC = Tensor2(P2B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P2B,H6B,0,1,iV_tensor, dist_nw, dim_ov);
        tB = Tensor2(H6B,H1B,0,1,iV_tensor, dist_nw, dim_ov);
	m_x1_1 = Multiplication(tC,tA,tB,-1.0);
#endif

	//i1 ( h6 p7 )_f + = 1 * f ( h6 p7 )_f
        tC = Tensor2(H6B,P7B,0,1,iV_tensor, dist_nw, dim_ov);
        tA = Tensor2(H6B,P7B,0,1,V_tensor, dist_nw, dim_n);
	a_x1_2_1 = Assignment(tC,tA,1.0,ivec(H6B,P7B),ivec(H6B,P7B));

	//i1 ( h6 p7 )_vt + = 1 * Sum ( h4 p3 ) * t ( p3 h4 )_t * v ( h4 h6 p3 p7 )_v
        tC = Tensor2(H6B,P7B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P3B,H4B,0,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H4B,H6B,P3B,P7B,0,0,1,1,iV_tensor, idist, dim_n);
	m_x1_2_2 = Multiplication(tC,tA,tB,1.0);

#if 0
	//i0 ( p2 h1 )_xf + = 1 * Sum ( p7 h6 ) * x ( p2 p7 h1 h6 )_x * i1 ( h6 p7 )_f
        tC = Tensor2(P2B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor4(P2B,P7B,H1B,H6B,0,0,1,1,iVT_tensor, dist_nw, dim_ov);
        tB = Tensor2(H6B,P7B,0,1,iVT_tensor, dist_nw, dim_ov);
	m_x1_2 = Multiplication(tC,tA,tB,1.0);
#endif

	//i1 ( h6 h8 h1 p7 )_v + = 1 * v ( h6 h8 h1 p7 )_v
        tC = Tensor4(H6B,H8B,H1B,P7B,0,0,1,2,iV_tensor, dist_nw, dim_ov);
        tA = Tensor4(H6B,H8B,H1B,P7B,0,0,1,2,V_tensor, idist, dim_n);
	a_x1_3_1 = Assignment(tC,tA,1.0,ivec(H6B,H8B,H1B,P7B),ivec(H6B,H8B,H1B,P7B));

	//i1 ( h6 h8 h1 p7 )_vt + = 1 * Sum ( p3 ) * t ( p3 h1 )_t * v ( h6 h8 p3 p7 )_v
        tC = Tensor4(H6B,H8B,H1B,P7B,0,0,1,2,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor2(P3B,H1B,0,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H6B,H8B,P3B,P7B,0,0,1,1,iV_tensor, idist, dim_n);
	m_x1_3_2 = Multiplication(tC,tA,tB,1.0);

#if 0
	//i0 ( p2 h1 )_xv + = -1/2 * Sum ( p7 h6 h8 ) * x ( p2 p7 h6 h8 )_x * i1 ( h6 h8 h1 p7 )_v
        tC = Tensor2(P2B,H1B,0,1,iVT_tensor, dist_nw, dim_ov);
        tA = Tensor4(P2B,P7B,H6B,H8B,0,0,1,1,T_tensor, dist_nw, dim_ov);
        tB = Tensor4(H6B,H8B,H1B,P7B,0,0,1,2,iVT_tensor, dist_nw, dim_ov);
	m_x1_3 = Multiplication(tC,tA,tB,-0.5);
#endif
      }
    }

#if 0
    void ipccsd_x1_1_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_1);
    }
#endif

    void ipccsd_x1_1_1_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_c, Integer *k_c_offset) {
      t_assign3(d_a, k_a_offset, d_c, k_c_offset, a_x1_1_1);
    }

#if 0
    void ipccsd_x1_1_2_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_1_2);
    }
#endif

    void ipccsd_x1_1_2_1_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_c, Integer *k_c_offset) {
      t_assign3(d_a, k_a_offset, d_c, k_c_offset, a_x1_1_2_1);
    }

    void ipccsd_x1_1_2_2_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_1_2_2);
    }

#if 0
    void ipccsd_x1_1_3_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_1_3);
    }
#endif

    void ipccsd_x1_1_4_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_1_4);
    }

    void ipccsd_x1_2_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_2);
    }

    void ipccsd_x1_2_1_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_c, Integer *k_c_offset) {
      t_assign3(d_a, k_a_offset, d_c, k_c_offset, a_x1_2_1);
    }

    void ipccsd_x1_2_2_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_2_2);
    }

    void ipccsd_x1_3_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_3);
    }

    void ipccsd_x1_3_1_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_c, Integer *k_c_offset) {
      t_assign3(d_a, k_a_offset, d_c, k_c_offset, a_x1_3_1);
    }
    
    void ipccsd_x1_3_2_cxx_(Integer *d_a, Integer *k_a_offset, Integer *d_b, Integer *k_b_offset, Integer *d_c, Integer *k_c_offset) {
      t_mult4(d_a, k_a_offset, d_b, k_b_offset, d_c, k_c_offset, m_x1_3_2);
    }
    
  } /*extern "C"*/

} /*ctce*/

