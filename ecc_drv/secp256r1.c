/*
 * Copyright 2018 The OpenWallet Authors
 * This file is part of the OpenWallet library.
 *
 * The OpenWallet library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The OpenWallet library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */

#include "secp256r1.h"

//////////////////////////////////////////////////////////////////////////////////////SECP256R1 CURVE/////////////////////////////////////////////////////////////////////////////////////////////////////////
static const uint8_ow curve_secp256r1_p[32] = {0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_ow curve_secp256r1_a[32] = {0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC};
static const uint8_ow curve_secp256r1_b[32] = {0x5A,0xC6,0x35,0xD8,0xAA,0x3A,0x93,0xE7,0xB3,0xEB,0xBD,0x55,0x76,0x98,0x86,0xBC,0x65,0x1D,0x06,0xB0,0xCC,0x53,0xB0,0xF6,0x3B,0xCE,0x3C,0x3E,0x27,0xD2,0x60,0x4B};
static const uint8_ow curve_secp256r1_x[32] = {0x6B,0x17,0xD1,0xF2,0xE1,0x2C,0x42,0x47,0xF8,0xBC,0xE6,0xE5,0x63,0xA4,0x40,0xF2,0x77,0x03,0x7D,0x81,0x2D,0xEB,0x33,0xA0,0xF4,0xA1,0x39,0x45,0xD8,0x98,0xC2,0x96};
static const uint8_ow curve_secp256r1_y[32] = {0x4f,0xe3,0x42,0xe2,0xfe,0x1a,0x7f,0x9b,0x8e,0xe7,0xeb,0x4a,0x7c,0x0f,0x9e,0x16,0x2b,0xce,0x33,0x57,0x6b,0x31,0x5e,0xce,0xcb,0xb6,0x40,0x68,0x37,0xbf,0x51,0xf5};
static const uint8_ow curve_secp256r1_n[32] = {0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBC,0xE6,0xFA,0xAD,0xA7,0x17,0x9E,0x84,0xF3,0xB9,0xCA,0xC2,0xFC,0x63,0x25,0x51};

void secp256r1_get_order(uint8_ow *order)
{
    memcpy(order, curve_secp256r1_n, ECC_LEN);
}

uint16_ow secp256r1_genPubkey(uint8_ow *prikey, uint8_ow *pubkey)
{
    uint16_ow ret = 0;
    ECC_CURVE_PARAM *curveParam = NULL;
    ECC_POINT *point = NULL;
    
    curveParam = calloc(1, sizeof(ECC_CURVE_PARAM));
    point = calloc(1, sizeof(ECC_POINT));
    
    curveParam -> p = (uint8_ow *)curve_secp256r1_p;
    curveParam -> a = (uint8_ow *)curve_secp256r1_a;
    curveParam -> b = (uint8_ow *)curve_secp256r1_b;
    curveParam -> x = (uint8_ow *)curve_secp256r1_x;
    curveParam -> y = (uint8_ow *)curve_secp256r1_y;
    curveParam -> n = (uint8_ow *)curve_secp256r1_n;
    
    ret = ECDSA_genPubkey(curveParam, prikey, point);
    
    memcpy(pubkey, point -> x, ECC_LEN);
    memcpy(pubkey + ECC_LEN, point -> y, ECC_LEN);
    
    free(curveParam);
    free(point);
    return ret;
}


uint16_ow secp256r1_sign(uint8_ow *prikey, uint8_ow *message, uint16_ow message_len,uint8_ow *rand,uint8_ow hash_flag, uint8_ow *sig)
{
    uint16_ow ret = 0;
    ECC_CURVE_PARAM *curveParam = NULL;
    
    curveParam = calloc(1, sizeof(ECC_CURVE_PARAM));
    
    curveParam -> p = (uint8_ow *)curve_secp256r1_p;
    curveParam -> a = (uint8_ow *)curve_secp256r1_a;
    curveParam -> b = (uint8_ow *)curve_secp256r1_b;
    curveParam -> x = (uint8_ow *)curve_secp256r1_x;
    curveParam -> y = (uint8_ow *)curve_secp256r1_y;
    curveParam -> n = (uint8_ow *)curve_secp256r1_n;
    
    ret = ECDSA_sign(curveParam, prikey, message, message_len, rand,hash_flag,sig);
    
    free(curveParam);
    return ret;
}



uint16_ow secp256r1_verify(uint8_ow *pubkey, uint8_ow *message, uint16_ow message_len,uint8_ow hash_flag, uint8_ow *sig)
{
    uint16_ow ret = 0;
    ECC_CURVE_PARAM *curveParam = NULL;
    ECC_POINT *point = NULL;
    
    curveParam = calloc(1, sizeof(ECC_CURVE_PARAM));
    point = calloc(1, sizeof(ECC_POINT));
    
    curveParam -> p = (uint8_ow *)curve_secp256r1_p;
    curveParam -> a = (uint8_ow *)curve_secp256r1_a;
    curveParam -> b = (uint8_ow *)curve_secp256r1_b;
    curveParam -> x = (uint8_ow *)curve_secp256r1_x;
    curveParam -> y = (uint8_ow *)curve_secp256r1_y;
    curveParam -> n = (uint8_ow *)curve_secp256r1_n;
    
    memcpy(point -> x, pubkey, ECC_LEN);
    memcpy(point -> y, pubkey + ECC_LEN, ECC_LEN);
    
    ret = ECDSA_verify(curveParam, point, message, message_len,hash_flag, sig);
    
    free(curveParam);
    free(point);
    return ret;
}
/*
 @function:(Point)outpoint_buf = (Point)inputpoint1_buf+[k](Point)inputpoint2_buf
 @paramter[in]:inputpoint1_buf pointer to one point(stored by byte string)on the curve elliptic
 @paramter[in]:inputpoint2_buf pointer to another point(stored by byte string)on the curve elliptic
 @paramter[in]:k pointer to the multiplicator
 @paramter[out]:outpoint_buf pointer to the result(stored by byte string)
 @return:0表示运算失败；1表示运算成功.
 */

uint16_ow secp256r1_point_mul_add(uint8_ow *inputpoint1_buf,uint8_ow *inputpoint2_buf,uint8_ow *k,uint8_ow *outpoint_buf)
{
    uint16_ow ret;
    ECC_CURVE_PARAM *curveParam;
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a =(uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->p=(uint8_ow *)curve_secp256r1_p;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->x =(uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    ret=point_mul_add(curveParam,inputpoint1_buf,inputpoint2_buf,k,outpoint_buf);
    free(curveParam);
    return ret;
}

/*
 @function:(Point)outpoint_buf = (Point)inputpoint_buf+[k]G(G is the base point of curve elliptic)
 @paramter[in]:inputpoint_buf pointer to the point on curve elliptic(stored by byte string)
 @paramter[in]:k pointer to the multiplicator
 @paramter[out]:outpoint_buf pointer to the result(stored by byte string)
 @return:0 表示运算失败；1表示运算成功.
 */

uint16_ow secp256r1_point_mul_base_G_add(uint8_ow *inputpoint_buf,uint8_ow *k,uint8_ow *outpoint_buf)
{
    uint16_ow ret;
    uint8_ow *temp_point_buf =NULL;
    ECC_CURVE_PARAM *curveParam;
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    temp_point_buf=calloc(ECC_LEN<<1,sizeof(uint8_ow));
    curveParam->a =(uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->p=(uint8_ow *)curve_secp256r1_p;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->x =(uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    memcpy(temp_point_buf,curve_secp256r1_x,ECC_LEN);
    memcpy(temp_point_buf + ECC_LEN,curve_secp256r1_y,ECC_LEN);
    ret=point_mul_add(curveParam,inputpoint_buf,temp_point_buf,k,outpoint_buf);
    free(curveParam);
    free(temp_point_buf);
    return ret;
}

/*
 @function:椭圆曲线（参数为secp256r1）上点的压缩
 @paramter[in]:point_buf,待压缩的点(stored by byte string)
 @paramter[in]:point_buf_len表示point_buf的字节长度
 @paramter[out]:x,点压缩后的横坐标（长度为ECC_LEN+1 字节）
 @return：0表示压缩失败；1表示压缩成功
 */

uint16_ow secp256r1_point_compress(uint8_ow *point_buf,uint16_ow point_buf_len,uint8_ow *x)
{
    return point_compress(point_buf,point_buf_len,x);
}

/*
 @function:椭圆曲线(参数为secp256r1)点的解压缩
 @paramter[in]:x pointer to the x-coordiate of the point on curve elliptic
 @paramter[in]:x_len denotes the byte length of x(x_len=ECC_LEN=1)
 @paramter[out]:point_buf pointer to the xy-coordiate(with 0x04) of the point on curve elliptic
 @return：0 表示解压缩失败；1 表示解压缩成功.
 */
uint16_ow secp256r1_point_decompress(uint8_ow *x,uint16_ow x_len,uint8_ow *point_buf)
{
    uint16_ow ret;
    ECC_CURVE_PARAM *curveParam=NULL;
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    ret=point_decompress(curveParam, x,x_len,point_buf);
    free(curveParam);
    return ret;
}

/*
 @function:recover the the public key of the signer.
 @paramter[in]sig pointer to signature(r||s||v)
 @paramter[in]sig_len denotes the length of sig(65 bytes)
 @paramter[in]msg pointer to message(or hash value)
 @paramter[in]msg_len denotes the length of msg
 @paramter[in]hash_flag denotes the message type.1:hash(msg);0:just msg
 @paramter[out]pubkey pointer to the recovery public
 */
uint16_ow secp256r1_recover_pubkey(uint8_ow *sig,uint32_ow sig_len,uint8_ow *msg,uint32_ow msg_len,uint8_ow hash_flag,uint8_ow *pubkey)
{
    uint16_ow ret;
    ECC_CURVE_PARAM *curveParam=NULL;
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    ret=ECDSA_recover_public(curveParam,sig,sig_len,msg,msg_len,hash_flag,pubkey);
    free(curveParam);
    return ret;
}

uint16_ow secp256r1_multisig_keyexchange_step1(uint8_ow *pubkey, uint8_ow *tmp_rand, uint8_ow *tmp_point)
{
    ECC_CURVE_PARAM *curveParam=NULL;
    ECC_POINT *tmppub_point = NULL;
    ECC_POINT *pub_point = NULL;
    
    tmppub_point = calloc(1, sizeof(ECC_POINT));
    pub_point = calloc(1, sizeof(ECC_POINT));
    memcpy(pub_point->x, pubkey, ECC_LEN);
    memcpy(pub_point->y, pubkey + ECC_LEN, ECC_LEN);
    
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    //bigrand_get_rand_range(tmp_rand, (uint8_ow*)curve_secp256k1_n, prikey, ECC_LEN, 0, 0);
    
    if(1 == point_mul(curveParam, pub_point, tmp_rand, tmppub_point))
    {
        free(tmppub_point);
        free(pub_point);
        free(curveParam);
        return POINT_AT_INFINITY;
    }
    
    memcpy(tmp_point, tmppub_point->x, ECC_LEN);
    memcpy(tmp_point + ECC_LEN, tmppub_point->y, ECC_LEN);
    
    free(tmppub_point);
    free(pub_point);
    free(curveParam);
    return SUCCESS;
}

uint16_ow secp256r1_multisig_keyexchange_step2(uint8_ow *prikey, uint8_ow *tmp_rand, uint8_ow *tmp_point, uint8_ow *result)
{
    ECC_CURVE_PARAM *curveParam=NULL;
    ECC_POINT *in_point = NULL;
    ECC_POINT *ret_point = NULL;
    SHA256_CTX *sha_ctx = NULL;
    
    in_point = calloc(1, sizeof(ECC_POINT));
    ret_point = calloc(1, sizeof(ECC_POINT));
    memcpy(in_point->x, tmp_point, ECC_LEN);
    memcpy(in_point->y, tmp_point + ECC_LEN, ECC_LEN);
    
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    
    if(1 == point_mul(curveParam, in_point, prikey, ret_point))
    {
        free(in_point);
        free(ret_point);
        free(curveParam);
        free(sha_ctx);
        return POINT_AT_INFINITY;
    }
    
    if(1 == point_mul(curveParam, ret_point, tmp_rand, ret_point))
    {
        free(in_point);
        free(ret_point);
        free(curveParam);
        free(sha_ctx);
        return POINT_AT_INFINITY;
    }
    
    sha_ctx = calloc(1, sizeof(SHA256_CTX));
    sha256_init(sha_ctx);
    sha256_update(sha_ctx, ret_point->x, ECC_LEN);
    sha256_update(sha_ctx, ret_point->y, ECC_LEN);
    sha256_final(sha_ctx, result);
    
    free(in_point);
    free(ret_point);
    free(curveParam);
    free(sha_ctx);
    return SUCCESS;
}

// point = point1 + point2
uint16_ow secp256r1_point_add(uint8_ow *point1, uint8_ow *point2, uint8_ow *point)
{
    ECC_CURVE_PARAM *curveParam=NULL;
    ECC_POINT *P1 = NULL, *P2 = NULL, *P = NULL;
    
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    
    P1 = calloc(1, sizeof(ECC_POINT));
    P2 = calloc(1, sizeof(ECC_POINT));
    P = calloc(1, sizeof(ECC_POINT));
    
    memcpy(P1->x, point1, ECC_LEN);
    memcpy(P1->y, point1 + ECC_LEN, ECC_LEN);
    
    memcpy(P2->x, point2, ECC_LEN);
    memcpy(P2->y, point2 + ECC_LEN, ECC_LEN);
    
    point_add(curveParam, P1, P2, P);
    
    memcpy(point, P->x, ECC_LEN);
    memcpy(point + ECC_LEN, P->y, ECC_LEN);
    
    free(P1);
    free(P2);
    free(P);
    free(curveParam);
    return SUCCESS;
}


// point_out = scalar * point_in
uint16_ow secp256r1_point_mul(uint8_ow *point_in, uint8_ow *scalar, uint8_ow *point_out)
{
    ECC_CURVE_PARAM *curveParam=NULL;
    ECC_POINT *Pin = NULL, *Pout = NULL;
    
    curveParam = calloc(1,sizeof(ECC_CURVE_PARAM));
    curveParam->a = (uint8_ow *)curve_secp256r1_a;
    curveParam->b = (uint8_ow *)curve_secp256r1_b;
    curveParam->n =(uint8_ow *)curve_secp256r1_n;
    curveParam->p =(uint8_ow *)curve_secp256r1_p;
    curveParam->x = (uint8_ow *)curve_secp256r1_x;
    curveParam->y =(uint8_ow *)curve_secp256r1_y;
    
    Pin = calloc(1, sizeof(ECC_POINT));
    Pout = calloc(1, sizeof(ECC_POINT));
    
    memcpy(Pin->x, point_in, ECC_LEN);
    memcpy(Pin->y, point_in + ECC_LEN, ECC_LEN);
    
    point_mul(curveParam, Pin, scalar, Pout);
    
    memcpy(point_out, Pout->x, ECC_LEN);
    memcpy(point_out + ECC_LEN, Pout->y, ECC_LEN);
    
    free(Pin);
    free(Pout);
    free(curveParam);
    return SUCCESS;
}

