#ifndef ENCRYPT_H
#define ENCRYPT_H

/**
 * encryption key
 */
#define KEY "KEY"

/**
 * printable ascii range constants
 */
#define RANGE_MIN 33
#define RANGE_MAX 126
#define RANGE_SIZE (RANGE_MAX - RANGE_MIN + 1)

/**
 * encrypts a source string into a destination buffer using a shift cipher.
 * @param dest buffer where the encrypted string will be stored.
 * @param source the plaintext string to encrypt.
 * @return 1 if encryption succeeded, 0 if the source contains invalid characters.
 */
int encrypt(char dest[], const char source[]);

/**
 * decrypts an encrypted string back into its original plaintext.
 * @param dest buffer where the decrypted string will be stored.
 * @param source the encrypted string to decrypt.
 * @return 1 if decryption succeeded, 0 if the source contains invalid characters.
 */
int decrypt(char dest[], const char source[]);

/**
 * checks if an encrypted string matches a raw plaintext string.
 * @param encrypted the previously encrypted string to verify.
 * @param raw the plaintext string to compare against.
 * @return 1 if they match after encryption, 0 otherwise.
 */
int is_equal(const char encrypted[], const char raw[]);

/**
 * verifies if a string consists only of characters within the valid encryptable range.
 * @param s the string to validate.
 * @return 1 if all characters are valid, 0 otherwise.
 */
int is_encryptable(const char s[]);

#endif // ENCRYPT_H
