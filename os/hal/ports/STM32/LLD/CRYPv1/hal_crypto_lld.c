/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    CRYPv1/hal_crypto_lld.c
 * @brief   STM32 cryptographic subsystem low level driver source.
 *
 * @addtogroup CRYPTO
 * @{
 */

#include "hal.h"

#if (HAL_USE_CRY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CRY1 driver identifier.*/
#if STM32_CRY_USE_CRYP1 || defined(__DOXYGEN__)
CRYDriver CRYD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level crypto driver initialization.
 *
 * @notapi
 */
void cry_lld_init(void) {

}

/**
 * @brief   Configures and activates the crypto peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 *
 * @notapi
 */
void cry_lld_start(CRYDriver *cryp) {

  if (cryp->state == CRY_STOP) {

  }
}

/**
 * @brief   Deactivates the crypto peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 *
 * @notapi
 */
void cry_lld_stop(CRYDriver *cryp) {

  if (cryp->state == CRY_READY) {

  }
}

/**
 * @brief   Initializes the transient key for a specific algorithm.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] keyp      pointer to the key data
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the specified algorithm is unknown or
 *                              unsupported.
 * @retval CRY_ERR_INV_KEY_SIZE if the specified key size is invalid.
 *
 * @notapi
 */
cryerror_t cry_lld_loadkey(CRYDriver *cryp,
                           cryalgorithm_t algorithm,
                           size_t size,
                           const uint8_t *keyp) {

  (void)cryp;
  (void)algorithm;
  (void)size;
  (void)keyp;

  return CRY_NOERROR;
}

/**
 * @brief   Encryption of a single block using AES.
 * @note    The implementation of this function must guarantee that it can
 *          be called from any context.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES(CRYDriver *cryp,
                               crykey_t key_id,
                               const uint8_t *in,
                               uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption of a single block using AES.
 * @note    The implementation of this function must guarantee that it can
 *          be called from any context.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] in        buffer containing the input cyphertext
 * @param[out] out      buffer for the output plaintext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES(CRYDriver *cryp,
                               crykey_t key_id,
                               const uint8_t *in,
                               uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-ECB.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES_ECB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-ECB.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_ECB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CBC.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CBC.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CFB.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES_CFB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CFB.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_CFB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CTR.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector + counter, it contains
 *                      a 96 bits IV and a 32 bits counter
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES_CTR(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CTR.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
 * @param[in] in        buffer containing the input cyphertext
 * @param[out] out      buffer for the output plaintext
 * @param[in] iv        128 bits initial vector + counter, it contains
 *                      a 96 bits IV and a 32 bits counter
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_CTR(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-GCM.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the text buffers, this number must be a
 *                      multiple of 16
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out       buffer for the output cyphertext
 * @param[in] iv        128 bits initial vector + counter, it contains
 *                      a 96 bits IV and a 32 bits counter
 * @param[in] aadsize   size of the authentication data, this number must be a
 *                      multiple of 16
 * @param[in] aad       buffer containing the authentication data
 * @param[in] authtag   128 bits buffer for the generated authentication tag
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_AES_GCM(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv,
                                   size_t aadsize,
                                   const uint8_t *aad,
                                   uint8_t *authtag) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;
  (void)aadsize;
  (void)aad;
  (void)authtag;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-GCM.
 * @note    The function operates on data buffers whose lenght is a multiple
 *          of an AES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the text buffers, this number must be a
 *                      multiple of 16
 * @param[in] in        buffer for the output cyphertext
 * @param[out] out      buffer containing the input plaintext
 * @param[in] iv        128 bits initial vector + counter, it contains
 *                      a 96 bits IV and a 32 bits counter
 * @param[in] aadsize   size of the authentication data, this number must be a
 *                      multiple of 16
 * @param[in] aad       buffer containing the authentication data
 * @param[in] authtag   128 bits buffer for the generated authentication tag
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_GCM(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv,
                                   size_t aadsize,
                                   const uint8_t *aad,
                                   uint8_t *authtag) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;
  (void)aadsize;
  (void)aad;
  (void)authtag;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption of a single block using (T)DES.
 * @note    The implementation of this function must guarantee that it can
 *          be called from any context.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_DES(CRYDriver *cryp,
                               crykey_t key_id,
                               const uint8_t *in,
                               uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption of a single block using (T)DES.
 * @note    The implementation of this function must guarantee that it can
 *          be called from any context.
 *
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] in        buffer containing the input cyphertext
 * @param[out] out      buffer for the output plaintext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_DES(CRYDriver *cryp,
                               crykey_t key_id,
                               const uint8_t *in,
                               uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using (T)DES-ECB.
 * @note    The function operates on data buffers whose length is a multiple
 *          of an DES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 8
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_DES_ECB(CRYDriver *cryp,
                                  crykey_t key_id,
                                  size_t size,
                                  const uint8_t *in,
                                  uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using (T)DES-ECB.
 * @note    The function operates on data buffers whose length is a multiple
 *          of an DES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 8
 * @param[in] in        buffer containing the input cyphertext
 * @param[out] out      buffer for the output plaintext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_DES_ECB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using (T)DES-CBC.
 * @note    The function operates on data buffers whose length is a multiple
 *          of an DES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 8
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        64 bits input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_encrypt_DES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using (T)DES-CBC.
 * @note    The function operates on data buffers whose length is a multiple
 *          of an DES block, this means that padding must be done by the
 *          caller.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 8
 * @param[in] in        buffer containing the input cyphertext
 * @param[out] out      buffer for the output plaintext
 * @param[in] iv        64 bits input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers to an empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_DES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Hash using SHA1.
 * @NOTE    Use of this algorithm is not recommended because proven weak.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] size      size of input buffer
 * @param[in] in        buffer containing the input text
 * @param[out] out      160 bits output buffer
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 *
 * @api
 */
cryerror_t cry_lld_SHA1(CRYDriver *cryp, size_t size,
                        const uint8_t *in, uint8_t *out) {

  (void)cryp;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Hash using SHA256.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] size      size of input buffer
 * @param[in] in        buffer containing the input text
 * @param[out] out      256 bits output buffer
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 *
 * @api
 */
cryerror_t cry_lld_SHA256(CRYDriver *cryp, size_t size,
                          const uint8_t *in, uint8_t *out) {

  (void)cryp;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Hash using SHA512.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] size      size of input buffer
 * @param[in] in        buffer containing the input text
 * @param[out] out      512 bits output buffer
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 *
 * @api
 */
cryerror_t cry_lld_SHA512(CRYDriver *cryp, size_t size,
                          const uint8_t *in, uint8_t *out) {

  (void)cryp;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   True random numbers generator.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[out] out      128 bits output buffer
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 *
 * @api
 */
cryerror_t cry_lld_TRNG(CRYDriver *cryp, uint8_t *out) {

  (void)cryp;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

#endif /* HAL_USE_CRY == TRUE */

/** @} */
