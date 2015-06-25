// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------

#include <kortex/svd.h>
#include <kortex/kmatrix.h>

using namespace kortex;

void matrix_test();

int main(int argc, char **argv) {
    matrix_test();
    release_log_man();
}

bool mat_is_equal( const KMatrix& A, const KMatrix& B, double err_th ) {
    if( A.h() != B.h() ) return false;
    if( A.w() != B.w() ) return false;
    for( int y=0; y<A.h(); y++ ) {
        for( int x=0; x<A.w(); x++ ) {
            if( fabs( A(y,x)-B(y,x) ) > err_th )
                return false;
        }
    }
    return true;
}
bool mat_mag_is_equal( const KMatrix& A, const KMatrix& B, double err_th ) {
    if( A.h() != B.h() ) return false;
    if( A.w() != B.w() ) return false;
    for( int y=0; y<A.h(); y++ ) {
        for( int x=0; x<A.w(); x++ ) {
            if( fabs( fabs(A(y,x))-fabs(B(y,x)) ) > err_th )
                return false;
        }
    }
    return true;
}

void assert_matrix_equality( const KMatrix& A, const KMatrix& B, double eps, string str ) {
    if( mat_is_equal(A, B, eps) ) printf("%50s passed\n", str.c_str() );
    else {
        printf("%50s failed\n", str.c_str() );
        A.print("A");
        B.print("B");
    }
}

void assert_matrix_magnitude_equality( const KMatrix& A, const KMatrix& B, double eps, string str ) {
    if( mat_mag_is_equal(A, B, eps) ) printf("%50s passed\n", str.c_str() );
    else {
        printf("%50s failed\n", str.c_str() );
        A.print("A");
        B.print("B");
    }
}


void assert_value_equality( const double& a, const double& b, double eps, string str ) {
    if( fabs(a-b) < eps ) printf("%50s passed\n", str.c_str() );
    else {
        printf("%50s failed [a %f] [b %f]\n", str.c_str(), a, b );
    }
}

void mat_copy_test() {

    KMatrix A(5,9);

    for( int y=0; y<5; y++ ) {
        for( int x=0; x<9; x++ ) {
            A.set( y, x, y*9+x );
        }
    }

    double top_left[] = {
        0.0000,    1.0000,    2.0000,    3.0000,
        9.0000,   10.0000,   11.0000,   12.0000,
        18.0000,   19.0000,   20.0000,   21.0000
    };
    double top_right[] = {
        5.0000,    6.0000,    7.0000,    8.0000,
        14.0000,   15.0000,   16.0000,   17.0000,
        23.0000,   24.0000,   25.0000,   26.0000
    };
    double bottom_left[] = {
        18.0000,   19.0000,   20.0000,   21.0000,
        27.0000,   28.0000,   29.0000,   30.0000,
        36.0000,   37.0000,   38.0000,   39.0000 };

    double bottom_right[] = {
        23.0000,   24.0000,   25.0000,   26.0000,
        32.0000,   33.0000,   34.0000,   35.0000,
        41.0000,   42.0000,   43.0000,   44.0000
    };

    KMatrix TL(top_left,3,4);
    KMatrix TR(top_right,3,4);
    KMatrix BL(bottom_left,3,4);
    KMatrix BR(bottom_right,3,4);

    KMatrix B;

    mat_top_left( A, 3, 4, B );
    assert_matrix_equality( TL, B, 1e-12, "top_left failed" );

    mat_top_right( A, 3, 4, B );
    assert_matrix_equality( TR, B, 1e-12, "top_right failed" );

    mat_bottom_left( A, 3, 4, B );
    assert_matrix_equality( BL, B, 1e-12, "bottom_left failed" );

    mat_bottom_right( A, 3, 4, B );
    assert_matrix_equality( BR, B, 1e-12, "bottom_right failed" );

}

void matrix_test() {

    double eps = 1e-10;

    // define comparison matrices
    double z_data[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    KMatrix gZ( (const double*)z_data, 4, 4 );

    double i_data[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
    KMatrix gI( (const double*)i_data, 3, 3 );
    double a_data[] = {
        0.3436688804605960,  0.0863212285183167,  0.5030670008016513,  0.3496723837404127,
        0.8514830781674081,  0.7407569272991721,  0.1272223460018265,  0.1044654315044041,
        0.6738894813341811,  0.1784195904359802,  0.6167043970549876,  0.7023864074250225,
        0.0118512002129784,  0.3097169911935462,  0.8558554306982441,  0.8949534548453012 };
    KMatrix gA( (const double*) a_data, 4, 4 );

    double at_data[] = {
        0.3436688804605960,   0.8514830781674081,   0.6738894813341811,   0.0118512002129784,
        0.0863212285183167,   0.7407569272991721,   0.1784195904359802,   0.3097169911935462,
        0.5030670008016513,   0.1272223460018265,   0.6167043970549876,   0.8558554306982441,
        0.3496723837404127,   0.1044654315044041,   0.7023864074250225,   0.8949534548453012 };
    KMatrix gAt( (const double*) at_data, 4, 4 );

    double b_data[] = {
        0.73118301679178210, 0.71978909927724144, 0.51242594768108685, 0.01335887621017422,
        0.45431376547891594, 0.53069669311093304, 0.24546935082099669, 0.82698809837566178,
        0.00502015374098324, 0.38104151340781584, 0.22613368480313550, 0.22259678233935515 };
    KMatrix gB( (const double*)b_data, 3, 4 );

    double c_data[] = {
        0.6824365555016827, 0.4859748704863389, 0.6129127879432523,
        0.8080734763838867, 0.0711821018793324, 0.6113789514527007,
        0.9308892831375858, 0.4703196994250146, 0.8703888508569482 };
    KMatrix gC( (const double*) c_data, 3, 3 );

    double d_data[] = {
        0.82875517369072471, 0.00612561878701139, 0.27082318442193859, 0.54135346473258217,
        0.03233030421303233, 0.64211390605921370, 0.53610455719155614, 0.81664332050594446,
        0.84690700974183131, 0.37278337881060125, 0.80382448176738164, 0.47248852221111365 };
    KMatrix gD( (const double*)d_data, 3, 4 );
    KMatrix gDr( (const double*)d_data, 6, 2 );

    double md_data[] = {
        -0.82875517369072471, -0.00612561878701139, -0.27082318442193859, -0.54135346473258217,
        -0.03233030421303233, -0.64211390605921370, -0.53610455719155614, -0.81664332050594446,
        -0.84690700974183131, -0.37278337881060125, -0.80382448176738164, -0.47248852221111365 };
    KMatrix gmD( (const double*)md_data, 3, 4 );

    double d_normalized[] = {
        0.40698914492576821, 0.00300819884015270, 0.13299717425968210, 0.26585050773551688,
        0.01587692395120097, 0.31533243817734402, 0.26327284854287114, 0.40104119681319361,
        0.41590323737163876, 0.18306828530434471, 0.39474605877632879, 0.23203197487814747 };
    KMatrix gDnormalized( (const double*)d_normalized, 3, 4 );

    double dt_data[] = {
        0.82875517369072471, 0.03233030421303233, 0.84690700974183131,
        0.00612561878701139, 0.64211390605921370, 0.37278337881060125,
        0.27082318442193859, 0.53610455719155614, 0.80382448176738164,
        0.54135346473258217, 0.81664332050594446, 0.47248852221111365 };
    KMatrix gDt( (const double*)dt_data, 4, 3 );

    double dbt_data[] = {
        0.756389562562933, 0.893937392868694, 0.188240377419911,
        0.771449282245571, 1.162407373094657, 0.547847832063679,
        1.305781872302604, 1.170653077188965, 0.433243762961991 };
    KMatrix gDBt( (const double*)dbt_data, 3, 3 );

    double btd_data[] = {
        0.624911413720303, 0.298071564826252, 0.445616315550131, 0.769212726508464,
        0.936393254233042, 0.487222823010658, 0.785734988688618, 1.003087973985261,
        0.624125956859617, 0.245057088769361, 0.452145856554440, 0.584710038487718,
        0.226326789884095, 0.614082770128397, 0.625898724884928, 0.787760605334179 };
    KMatrix gBtD( (const double*)btd_data, 4, 4 );

    double cb_data[] = {
        0.722848009184780,  0.982660866605434,  0.607590361859336,  0.547444433918912,
        0.626257827321590,  0.852379346719455,  0.569804216373434,  0.205752691994689,
        0.898692633814109,  1.251295352810044,  0.789285132449978,  0.595130186139724
    };
    KMatrix gCB( (const double*)cb_data, 3, 4 );

    double rscb_data[] = {
        0.722848009184780,  0.982660866605434,  0.607590361859336,  0.547444433918912,
        2.004025047429088,  2.727613909502256,  1.823373492394989,  0.658408614383005,
        0.898692633814109,  1.251295352810044,  0.789285132449978,  0.595130186139724
    };
    KMatrix grsCB( (const double*)rscb_data, 3, 4 );

    double cscb_data[] = {
        0.722848009184780,   0.982660866605434,   3.098710845482613,   0.547444433918912,
        0.626257827321590,   0.852379346719455,   2.906001503504513,   0.205752691994689,
        0.898692633814109,   1.251295352810044,   4.025354175494887,   0.595130186139724
    };
    KMatrix gcsCB( (const double*)cscb_data, 3, 4 );

    double b_elem_d_data[] = {
        0.60597170808098144,  0.00440915362921868,  0.13877682693142163,  0.00723187392131148,
        0.01468810224610158,  0.34076772654616905,  0.13159723762598918,  0.67535430667639706,
        0.00425160339322038,  0.14204594283527061,  0.18177179199702881,  0.10517442473647083
    };
    KMatrix gBeD( (const double*) b_elem_d_data, 3, 4 );

    double f_data[] = {
        0.575181073341498,   0.584280470929579,
        0.831671005818728,   0.673811406841386
    };
    KMatrix gF( (const double*)f_data, 2, 2 );
    double pf1_data[] = {
        0.575181073341498,   0.584280470929579,   0.000000000000000,   0.000000000000000, 0.0,
        0.831671005818728,   0.673811406841386,   0.000000000000000,   0.000000000000000, 0.0,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000, 0.0,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000
    };
    KMatrix gPF1( (const double*) pf1_data, 4, 5 );
    double pf2_data[] = {
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,
        0.000000000000000,   0.000000000000000,   0.575181073341498,   0.584280470929579,   0.000000000000000,
        0.000000000000000,   0.000000000000000,   0.831671005818728,   0.673811406841386,   0.000000000000000
    };
    KMatrix gPF2( (const double*) pf2_data, 4, 5 );

    double pf3_data[] = {
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.575181073341498,   0.584280470929579,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.831671005818728,   0.673811406841386,
        0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000,   0.000000000000000
    };
    KMatrix gPF3( (const double*) pf3_data, 4, 5 );

    double badt_data[] = {
        1.600273530119974,  1.479095866766053,  2.246911063543456,
        1.576470785195689,  2.035061718991268,  2.397318310892820,
        0.720705381501873,  0.797831415600016,  1.048701963990095
    };
    KMatrix gBADt( (const double*) badt_data, 3, 3 );


    double btcbadt_data[] = {
        2.53468043645853,  2.66435235856603,  3.65744782882551,
        3.72430213091357,  3.91438180968127,  5.37375899970451,
        2.27806448655484,  2.40810955090623,  3.29143132524610,
        2.19360179396958,  2.21896154231734,  3.13727035324598
    };
    KMatrix gBtCBADt( (const double*)btcbadt_data, 4, 3 );
    double bmd_data[] = {
        -0.09757215689894261,   0.71366348049023010,   0.24160276325914826,  -0.52799458852240799,
        0.42198346126588360,  -0.11141721294828066,  -0.29063520637055945,   0.01034477786971733,
        -0.84188685600084812,   0.00825813459721458,  -0.57769079696424619,  -0.24989173987175850 };
    KMatrix gBmD( (const double*)bmd_data, 3, 4);
    double bpd_data[] = {
        1.559938190482507,   0.725914718064253,   0.783249132103025,   0.554712340942756,
        0.486644069691948,   1.172810599170147,   0.781573908012553,   1.643631418881606,
        0.851927163482814,   0.753824892218417,   1.029958166570517,   0.695085304550469 };
    KMatrix gBpD( (const double*)bpd_data, 3, 4 );
    double babt_data[] = {
        1.795068942537757,   1.691043614703065,   0.604066915830121,
        1.706784955972690,   1.959892299876727,   0.795020063703294,
        0.834548728830690,   0.850627936086273,   0.326878604002087
    };
    KMatrix gBABt( (const double*)babt_data, 3, 3 );
    double inv_a_data[] = {
        0.3589142333326330,   0.1243466273159543,   1.1625351295910842,  -1.0671406290625616,
        -0.6299132626652394,   1.2687859802718349,  -1.3016025107370501,   1.1195519757786720,
        5.0777158606169728,   0.0110036074415940,  -2.6044619971456719,   0.0588323332651078,
        -4.6426430295000989,  -0.4512589208457171,   2.9257319550064929,   0.6878018656381126
    };
    KMatrix gAinv( (const double*)inv_a_data, 4, 4 );
    double pinv_b_data[] = {
        0.9222035726200732,   0.7468382656668817,  -2.8164142517907296,
        0.2434184080676174,  -0.4420566985958504,   1.9551124778296767,
        0.3133873030667279,  -0.4766350702641631,   1.2747041997852020,
        -0.7558489667336609,   1.2240785891575365,  -0.0857772624708925 };
    KMatrix gB_pinv( (const double*)pinv_b_data, 4, 3 );
    double cinv_data[] = {
        8.402780372669305,    5.018189668047080,   -9.441971911792878,
        4.999188487680619,   -0.872776474227957,   -2.907286077114467,
        -11.688195470743301,   -4.895392455357824,   12.818160948776629 };
    KMatrix gCinv( (const double*)cinv_data, 3, 3 );

    double   g_data[] = { 0.724106275974106,  0.213387624220311, 0.504472749633369 };
    KMatrix gG( (const double*)  g_data, 3, 1);

    double csg_data[] = { 2.39210804233016, 1.96705745910340, -3.04169896177976 };
    KMatrix gS( (const double*)csg_data, 3, 1);

    double concat_h_bd_data[] = {
        0.73118301679178210,  0.71978909927724144,  0.51242594768108685,  0.01335887621017422,  0.82875517369072471,  0.00612561878701139,  0.27082318442193859,  0.54135346473258217,
        0.45431376547891594,  0.53069669311093304,  0.24546935082099669,  0.82698809837566178,  0.03233030421303233,  0.64211390605921370,  0.53610455719155614,  0.81664332050594446,
        0.00502015374098324,  0.38104151340781584,  0.22613368480313550,  0.22259678233935515,  0.84690700974183131,  0.37278337881060125,  0.80382448176738164,  0.47248852221111365
    };
    KMatrix gCH_BD( (const double*)concat_h_bd_data, 3, 8 );
    double concat_v_bd_data[] = {
        0.73118301679178210,  0.71978909927724144,  0.51242594768108685,  0.01335887621017422,
        0.45431376547891594,  0.53069669311093304,  0.24546935082099669,  0.82698809837566178,
        0.00502015374098324,  0.38104151340781584,  0.22613368480313550,  0.22259678233935515,
        0.82875517369072471,  0.00612561878701139,  0.27082318442193859,  0.54135346473258217,
        0.03233030421303233,  0.64211390605921370,  0.53610455719155614,  0.81664332050594446,
        0.84690700974183131,  0.37278337881060125,  0.80382448176738164,  0.47248852221111365 };
    KMatrix gCV_BD( (const double*)concat_v_bd_data, 6, 4 );


    //

    KMatrix E( 3, 3 );
    E.identity();
    assert_matrix_equality( E, gI, eps, "identity" );

    E.copy( gA );   assert_matrix_equality( E, gA,  eps, "square copy" );
    E.transpose();  assert_matrix_equality( E, gAt, eps, "square transpose" );
    E.copy( gD );   assert_matrix_equality( E, gD,  eps, "non-square copy" );
    E.transpose();  assert_matrix_equality( E, gDt, eps, "non-square transpose" );

    E.copy( gD );
    E.negate();
    assert_matrix_equality( E, gmD, eps, "negate" );

    E.negate();
    assert_matrix_equality( E, gD, eps, "double negate" );

    E.normalize();
    assert_matrix_equality( E, gDnormalized, eps, "normalize" );

    assert_value_equality( gDBt.trace(), 2.35204069861958, eps, "trace" );
    assert_value_equality( gDBt.det(),  0.120817507913528, eps, "det" );
    assert_value_equality( gD.norm(),    2.03630780826325, eps, "norm" );
    assert_value_equality( gD.norm_sq(), 4.14654948999388, eps, "norm_sq" );
    assert_value_equality( gD(1,1), 0.64211390605921370, eps, "operator ()" );
    assert_value_equality( gD(1,2), 0.53610455719155614, eps, "operator ()" );
    assert_value_equality( gD(2,0), 0.84690700974183131, eps, "operator ()" );

    KMatrix gEmpty;
    E.release(); assert_matrix_equality( E, gEmpty, eps, "release" );

    E.copy( gD );
    E.reset(); assert_matrix_equality( E, gEmpty, eps, "reset" );

    E.copy( gD );
    E.resize(6,2); assert_matrix_equality( E, gDr, eps, "resize with mem intact" );

    E.resize(5,5); assert_matrix_equality( E, E, eps, "resize with larger mem size" );
    E.resize(1,1); assert_matrix_equality( E, E, eps, "resize with smaller mem size" );

    E.resize(3,4);
    E.set( cb_data, 12 ); assert_matrix_equality( E, gCB, eps, "set from a raw array" );

    E.resize(4,4);
    E.zero();
    assert_matrix_equality( E, gZ, eps, "zero" );

    E.resize(4,5);
    E.zero();
    E.copy( gF, 0, 0 );
    assert_matrix_equality( E, gPF1, eps, "copy sub matrix 1" );

    E.resize(4,5);
    E.zero();
    E.copy( gF, 2, 2 );
    assert_matrix_equality( E, gPF2, eps, "copy sub matrix 2" );

    E.resize(4,5);
    E.zero();
    E.copy( gF, 1, 3 );
    assert_matrix_equality( E, gPF3, eps, "copy sub matrix 3" );

    KMatrix ZF(4,4);
    ZF.zero();
    ZF.copy( gF, 2, 1 );

    E.resize(4,5);
    E.zero();
    E.copy( ZF, 2, 1, 2, 2, 0, 0 );
    assert_matrix_equality( E, gPF1, eps, "copy from sub matrix to sub matrix 1" );

    E.resize(4,5);
    E.zero();
    E.copy( ZF, 2, 1, 2, 2, 2, 2 );
    assert_matrix_equality( E, gPF2, eps, "copy from sub matrix to sub matrix 2" );

    E.resize(4,5);
    E.zero();
    E.copy( ZF, 2, 1, 2, 2, 1, 3 );
    assert_matrix_equality( E, gPF3, eps, "copy from sub matrix to sub matrix 3" );

    E.copy( gCB );
    E.scale_row( 1, 3.2 );
    assert_matrix_equality( E, grsCB, eps, "scale row" );

    E.copy( gCB );
    E.scale_col( 2, 5.1 );
    assert_matrix_equality( E, gcsCB, eps, "scale col" );

    E.release();
    mat_mat( gC, gB, E ); assert_matrix_equality( E, gCB, eps, "mat_mat" );

    E.release();
    mat_mat_elem( gB, gD, E ); assert_matrix_equality( E, gBeD, eps, "mat_mat_elem" );

    E.release();
    mat_mat_trans( gD, gB, E );
    assert_matrix_equality( E, gDBt, eps, "mat_mat_trans" );

    E.release();
    mat_trans_mat( gB, gD, E );
    assert_matrix_equality( E, gBtD, eps, "mat_trans_mat" );

    mat_mat_mat( gB, gA, gDt, E );
    assert_matrix_equality( E, gBADt, eps, "mat_mat_mat" );

    mat_mat_mat_trans( gB, gA, gD, E );
    assert_matrix_equality( E, gBADt, eps, "mat_mat_mat_trans" );

    mat_trans_mat_mat( gB, gC, gBADt, E );
    assert_matrix_equality( E, gBtCBADt, eps, "mat_trans_mat_mat" );

    mat_minus_mat( gB, gD, E );
    assert_matrix_equality( E, gBmD, eps, "mat_minus_mat" );

    mat_plus_mat( gB, gD, E );
    assert_matrix_equality( E, gBpD, eps, "mat_plus_mat" );

    mat_ABAt( gB, gA, E );
    assert_matrix_equality( E, gBABt, eps, "mat_ABAt" );

    mat_inv( gA, E );
    assert_matrix_equality( E, gAinv, eps, "mat inv" );

    mat_pseudo_inv( gB, E );
    assert_matrix_equality( E, gB_pinv, eps, "mat pseudo inv" );

    mat_inv_3( gC, E, eps );
    assert_matrix_equality( E, gCinv, eps, "mat_inv_3" );

    mat_solve_Ax_b_3( gC, gG, E );
    assert_matrix_equality( E, gS, eps, "mat_solve_Ax_b_3" );

    E.reset();
    KMatrix gQ(3,1); gQ.zero();
    mat_alpha_Ax_plus_beta_y( 1.0, gCinv, gG, -1.0, gS, E );
    assert_matrix_equality( E, gQ, eps, "mat_alpha_Ax_plus_beta_y" );

    KMatrix nS; nS.copy( gS ); nS.negate();
    mat_Ax_plus_y( gCinv, gG, nS, E );
    assert_matrix_equality( E, gQ, eps, "mat_Ax_plus_y" );

    mat_concat_hor( gB, gD, E );
    assert_matrix_equality( E, gCH_BD, eps, "mat_concat_hor" );

    mat_concat_ver( gB, gD, E );
    assert_matrix_equality( E, gCV_BD, eps, "mat_concat_ver" );

    E.init(3,4);
    KMatrix F(3,4);
    mat_deconcat_hor( gCH_BD, E, F );
    assert_matrix_equality( E, gB, eps, "mat_deconcat_hor 1" );
    assert_matrix_equality( F, gD, eps, "mat_deconcat_hor 2" );


    F.init(3,1);
    mat_vec_3( gCinv.get_const_pointer(), gG.get_const_pointer(), F.get_pointer() );
    mat_mat( gCinv, gG, E );
    assert_matrix_equality( E, F, eps, "mat_vec_3" );

    E.init(3,1);
    mat_trans_vec_3( gC.get_const_pointer(), gG.get_const_pointer(), E.get_pointer() );
    mat_trans_mat( gC, gG, F );
    assert_matrix_equality( E, F, eps, "mat_trans_vec_3" );

    double svd_u_data[] = {
        -0.491452475474706,   0.524651518772153,  -0.695136855734075,
        -0.525364516171787,  -0.815190841814656,  -0.243836044441952,
        -0.694598149650118,   0.245366410234951,   0.676260848512647
    };
    double svd_s_data[] = { 1.838032105376911, 0.802362704544172, 0.352706053186160 };
    double svd_v_data[] = {
        -0.5508821174742757,   0.7680501872190597,  -0.0319005188220302,  -0.3249771675959877,
        -0.3260492324860328,  -0.5343755866754204,   0.2587711351242880,  -0.7356440239593175,
        -0.5294151448927398,  -0.1217756036620034,   0.6368312439050957,   0.5471163253799344,
        -0.5567229068401321,  -0.3312285383701443,  -0.7255796464287440,   0.2321237537543075 };
    KMatrix gSVDU( (const double*) svd_u_data, 3, 3 );
    KMatrix gSVDV( (const double*) svd_v_data, 4, 4 );
    KMatrix gSVDS(3, 4); gSVDS.zero();
    gSVDS.set(0,0,svd_s_data[0]);
    gSVDS.set(1,1,svd_s_data[1]);
    gSVDS.set(2,2,svd_s_data[2]);

    KMatrix Dr;
    mat_mat_mat_trans( gSVDU, gSVDS, gSVDV, Dr );
    assert_matrix_equality( Dr, gD, eps, "svd input reconstructed" );

    {
        SVD svd;
        svd.decompose( gD.get_const_pointer(), 3, 4, true, true );

        KMatrix I;
        E.init(3,3);
        E.set( svd.U(), 9 );
        I.init(3,3);
        I.identity();
        mat_trans_mat( E, E, F );
        assert_matrix_equality( I, F, eps, "svd u identity" );

        I.init(4,4);
        I.identity();
        E.init(4,4);
        E.set( svd.Vt(), 16 );
        mat_trans_mat( E, E, F );
        assert_matrix_equality( I, F, eps, "svd vt identity" );

        E.init(3,3);
        E.set( svd.U(), 9 );
        assert_matrix_magnitude_equality( E, gSVDU, eps, "svd u ok" );

        E.init(4,4);
        E.set( svd.Vt(), 16 );
        E.transpose();
        assert_matrix_magnitude_equality( E, gSVDV, eps, "svd vt ok" );

        const double* sd = svd.Sd();

        assert_value_equality( sd[0], svd_s_data[0], eps, "svd s0" );
        assert_value_equality( sd[1], svd_s_data[1], eps, "svd s1" );
        assert_value_equality( sd[2], svd_s_data[2], eps, "svd s2" );
    }

    E.copy(gmD);
    F.init(3,4);
    F.zero();
    double md_data_shifted[] = {
        -0.54135346473258217, -0.82875517369072471, -0.00612561878701139, -0.27082318442193859,
        -0.81664332050594446, -0.03233030421303233, -0.64211390605921370, -0.53610455719155614,
        -0.47248852221111365, -0.84690700974183131, -0.37278337881060125, -0.80382448176738164 };
    KMatrix gmDs( (const double*) md_data_shifted, 3, 4 );

    mat_col_copy( gmD, 0, 1.0, F, 1 );
    mat_col_copy( gmD, 1, 1.0, F, 2 );
    mat_col_copy( gmD, 2, 1.0, F, 3 );
    mat_col_copy( gmD, 3, 1.0, F, 0 );
    assert_matrix_equality( F, gmDs, eps, "mat_col_copy" );

    mat_col_copy( E, 0, 2.0, F, 0 );
    mat_col_copy( E, 1, 2.0, F, 1 );
    mat_col_copy( E, 2, 2.0, F, 2 );
    mat_col_copy( E, 3, 2.0, F, 3 );
    F.scale( 0.5 );
    assert_matrix_equality( F, E, eps, "mat_col_copy + scale" );

    E.copy(gA);
    E.save("/tmp/kortex_matrix_tmp.tmp");
    F.load("/tmp/kortex_matrix_tmp.tmp");
    F.save("/tmp/kortex_matrix_tmp-2.tmp");
    assert_matrix_equality( F, gA, eps, "save/load" );

    mat_copy_test();

}






// Local Variables:
// mode: c++
// compile-command: "make -C ."
// End:
