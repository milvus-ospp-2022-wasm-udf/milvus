// Code generated from Plan.g4 by ANTLR 4.9. DO NOT EDIT.

package planparserv2

import (
	"fmt"
	"unicode"

	"github.com/antlr/antlr4/runtime/Go/antlr"
)

// Suppress unused import error
var _ = fmt.Printf
var _ = unicode.IsLetter

var serializedLexerAtn = []uint16{
	3, 24715, 42794, 33075, 47597, 16764, 15335, 30598, 22884, 2, 40, 454,
	8, 1, 4, 2, 9, 2, 4, 3, 9, 3, 4, 4, 9, 4, 4, 5, 9, 5, 4, 6, 9, 6, 4, 7,
	9, 7, 4, 8, 9, 8, 4, 9, 9, 9, 4, 10, 9, 10, 4, 11, 9, 11, 4, 12, 9, 12,
	4, 13, 9, 13, 4, 14, 9, 14, 4, 15, 9, 15, 4, 16, 9, 16, 4, 17, 9, 17, 4,
	18, 9, 18, 4, 19, 9, 19, 4, 20, 9, 20, 4, 21, 9, 21, 4, 22, 9, 22, 4, 23,
	9, 23, 4, 24, 9, 24, 4, 25, 9, 25, 4, 26, 9, 26, 4, 27, 9, 27, 4, 28, 9,
	28, 4, 29, 9, 29, 4, 30, 9, 30, 4, 31, 9, 31, 4, 32, 9, 32, 4, 33, 9, 33,
	4, 34, 9, 34, 4, 35, 9, 35, 4, 36, 9, 36, 4, 37, 9, 37, 4, 38, 9, 38, 4,
	39, 9, 39, 4, 40, 9, 40, 4, 41, 9, 41, 4, 42, 9, 42, 4, 43, 9, 43, 4, 44,
	9, 44, 4, 45, 9, 45, 4, 46, 9, 46, 4, 47, 9, 47, 4, 48, 9, 48, 4, 49, 9,
	49, 4, 50, 9, 50, 4, 51, 9, 51, 4, 52, 9, 52, 4, 53, 9, 53, 4, 54, 9, 54,
	4, 55, 9, 55, 4, 56, 9, 56, 4, 57, 9, 57, 4, 58, 9, 58, 4, 59, 9, 59, 4,
	60, 9, 60, 4, 61, 9, 61, 4, 62, 9, 62, 3, 2, 3, 2, 3, 3, 3, 3, 3, 4, 3,
	4, 3, 5, 3, 5, 3, 6, 3, 6, 3, 7, 3, 7, 3, 8, 3, 8, 3, 8, 3, 9, 3, 9, 3,
	10, 3, 10, 3, 10, 3, 11, 3, 11, 3, 11, 3, 12, 3, 12, 3, 12, 3, 13, 3, 13,
	3, 13, 3, 13, 3, 13, 3, 13, 3, 13, 3, 13, 5, 13, 160, 10, 13, 3, 14, 3,
	14, 3, 14, 3, 14, 3, 14, 3, 14, 5, 14, 168, 10, 14, 3, 15, 3, 15, 3, 16,
	3, 16, 3, 17, 3, 17, 3, 18, 3, 18, 3, 19, 3, 19, 3, 20, 3, 20, 3, 20, 3,
	21, 3, 21, 3, 21, 3, 22, 3, 22, 3, 22, 3, 23, 3, 23, 3, 24, 3, 24, 3, 25,
	3, 25, 3, 26, 3, 26, 3, 26, 3, 26, 3, 26, 5, 26, 200, 10, 26, 3, 27, 3,
	27, 3, 27, 3, 27, 5, 27, 206, 10, 27, 3, 28, 3, 28, 3, 29, 3, 29, 3, 29,
	3, 29, 5, 29, 214, 10, 29, 3, 30, 3, 30, 3, 30, 3, 31, 3, 31, 3, 31, 3,
	31, 3, 31, 3, 31, 3, 31, 3, 32, 3, 32, 3, 32, 7, 32, 229, 10, 32, 12, 32,
	14, 32, 232, 11, 32, 3, 32, 3, 32, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3,
	33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33,
	3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3, 33, 3,
	33, 5, 33, 263, 10, 33, 3, 34, 3, 34, 3, 34, 3, 34, 5, 34, 269, 10, 34,
	3, 35, 3, 35, 5, 35, 273, 10, 35, 3, 36, 3, 36, 3, 36, 7, 36, 278, 10,
	36, 12, 36, 14, 36, 281, 11, 36, 3, 37, 5, 37, 284, 10, 37, 3, 37, 3, 37,
	5, 37, 288, 10, 37, 3, 37, 3, 37, 3, 38, 3, 38, 3, 38, 5, 38, 295, 10,
	38, 3, 39, 6, 39, 298, 10, 39, 13, 39, 14, 39, 299, 3, 40, 3, 40, 3, 40,
	3, 40, 3, 40, 3, 40, 3, 40, 5, 40, 309, 10, 40, 3, 41, 3, 41, 3, 42, 3,
	42, 3, 43, 3, 43, 3, 43, 6, 43, 318, 10, 43, 13, 43, 14, 43, 319, 3, 44,
	3, 44, 7, 44, 324, 10, 44, 12, 44, 14, 44, 327, 11, 44, 3, 45, 3, 45, 7,
	45, 331, 10, 45, 12, 45, 14, 45, 334, 11, 45, 3, 46, 3, 46, 3, 46, 3, 46,
	3, 47, 3, 47, 3, 48, 3, 48, 3, 49, 3, 49, 3, 50, 3, 50, 3, 50, 3, 50, 3,
	50, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51, 3, 51,
	5, 51, 361, 10, 51, 3, 52, 3, 52, 5, 52, 365, 10, 52, 3, 52, 3, 52, 3,
	52, 5, 52, 370, 10, 52, 3, 53, 3, 53, 3, 53, 3, 53, 5, 53, 376, 10, 53,
	3, 53, 3, 53, 3, 54, 5, 54, 381, 10, 54, 3, 54, 3, 54, 3, 54, 3, 54, 3,
	54, 5, 54, 388, 10, 54, 3, 55, 3, 55, 5, 55, 392, 10, 55, 3, 55, 3, 55,
	3, 56, 6, 56, 397, 10, 56, 13, 56, 14, 56, 398, 3, 57, 5, 57, 402, 10,
	57, 3, 57, 3, 57, 3, 57, 3, 57, 3, 57, 5, 57, 409, 10, 57, 3, 58, 6, 58,
	412, 10, 58, 13, 58, 14, 58, 413, 3, 59, 3, 59, 5, 59, 418, 10, 59, 3,
	59, 3, 59, 3, 60, 3, 60, 3, 60, 3, 60, 3, 60, 5, 60, 427, 10, 60, 3, 60,
	5, 60, 430, 10, 60, 3, 60, 3, 60, 3, 60, 3, 60, 3, 60, 5, 60, 437, 10,
	60, 3, 61, 6, 61, 440, 10, 61, 13, 61, 14, 61, 441, 3, 61, 3, 61, 3, 62,
	3, 62, 5, 62, 448, 10, 62, 3, 62, 5, 62, 451, 10, 62, 3, 62, 3, 62, 2,
	2, 63, 3, 3, 5, 4, 7, 5, 9, 6, 11, 7, 13, 8, 15, 9, 17, 10, 19, 11, 21,
	12, 23, 13, 25, 14, 27, 15, 29, 16, 31, 17, 33, 18, 35, 19, 37, 20, 39,
	21, 41, 22, 43, 23, 45, 24, 47, 25, 49, 26, 51, 27, 53, 28, 55, 29, 57,
	30, 59, 31, 61, 32, 63, 33, 65, 34, 67, 35, 69, 36, 71, 37, 73, 38, 75,
	2, 77, 2, 79, 2, 81, 2, 83, 2, 85, 2, 87, 2, 89, 2, 91, 2, 93, 2, 95, 2,
	97, 2, 99, 2, 101, 2, 103, 2, 105, 2, 107, 2, 109, 2, 111, 2, 113, 2, 115,
	2, 117, 2, 119, 2, 121, 39, 123, 40, 3, 2, 17, 5, 2, 78, 78, 87, 87, 119,
	119, 6, 2, 12, 12, 15, 15, 36, 36, 94, 94, 5, 2, 67, 92, 97, 97, 99, 124,
	3, 2, 50, 59, 4, 2, 68, 68, 100, 100, 3, 2, 50, 51, 4, 2, 90, 90, 122,
	122, 3, 2, 51, 59, 3, 2, 50, 57, 5, 2, 50, 59, 67, 72, 99, 104, 4, 2, 71,
	71, 103, 103, 4, 2, 45, 45, 47, 47, 4, 2, 82, 82, 114, 114, 12, 2, 36,
	36, 41, 41, 65, 65, 94, 94, 99, 100, 104, 104, 112, 112, 116, 116, 118,
	118, 120, 120, 4, 2, 11, 11, 34, 34, 2, 478, 2, 3, 3, 2, 2, 2, 2, 5, 3,
	2, 2, 2, 2, 7, 3, 2, 2, 2, 2, 9, 3, 2, 2, 2, 2, 11, 3, 2, 2, 2, 2, 13,
	3, 2, 2, 2, 2, 15, 3, 2, 2, 2, 2, 17, 3, 2, 2, 2, 2, 19, 3, 2, 2, 2, 2,
	21, 3, 2, 2, 2, 2, 23, 3, 2, 2, 2, 2, 25, 3, 2, 2, 2, 2, 27, 3, 2, 2, 2,
	2, 29, 3, 2, 2, 2, 2, 31, 3, 2, 2, 2, 2, 33, 3, 2, 2, 2, 2, 35, 3, 2, 2,
	2, 2, 37, 3, 2, 2, 2, 2, 39, 3, 2, 2, 2, 2, 41, 3, 2, 2, 2, 2, 43, 3, 2,
	2, 2, 2, 45, 3, 2, 2, 2, 2, 47, 3, 2, 2, 2, 2, 49, 3, 2, 2, 2, 2, 51, 3,
	2, 2, 2, 2, 53, 3, 2, 2, 2, 2, 55, 3, 2, 2, 2, 2, 57, 3, 2, 2, 2, 2, 59,
	3, 2, 2, 2, 2, 61, 3, 2, 2, 2, 2, 63, 3, 2, 2, 2, 2, 65, 3, 2, 2, 2, 2,
	67, 3, 2, 2, 2, 2, 69, 3, 2, 2, 2, 2, 71, 3, 2, 2, 2, 2, 73, 3, 2, 2, 2,
	2, 121, 3, 2, 2, 2, 2, 123, 3, 2, 2, 2, 3, 125, 3, 2, 2, 2, 5, 127, 3,
	2, 2, 2, 7, 129, 3, 2, 2, 2, 9, 131, 3, 2, 2, 2, 11, 133, 3, 2, 2, 2, 13,
	135, 3, 2, 2, 2, 15, 137, 3, 2, 2, 2, 17, 140, 3, 2, 2, 2, 19, 142, 3,
	2, 2, 2, 21, 145, 3, 2, 2, 2, 23, 148, 3, 2, 2, 2, 25, 159, 3, 2, 2, 2,
	27, 167, 3, 2, 2, 2, 29, 169, 3, 2, 2, 2, 31, 171, 3, 2, 2, 2, 33, 173,
	3, 2, 2, 2, 35, 175, 3, 2, 2, 2, 37, 177, 3, 2, 2, 2, 39, 179, 3, 2, 2,
	2, 41, 182, 3, 2, 2, 2, 43, 185, 3, 2, 2, 2, 45, 188, 3, 2, 2, 2, 47, 190,
	3, 2, 2, 2, 49, 192, 3, 2, 2, 2, 51, 199, 3, 2, 2, 2, 53, 205, 3, 2, 2,
	2, 55, 207, 3, 2, 2, 2, 57, 213, 3, 2, 2, 2, 59, 215, 3, 2, 2, 2, 61, 218,
	3, 2, 2, 2, 63, 225, 3, 2, 2, 2, 65, 262, 3, 2, 2, 2, 67, 268, 3, 2, 2,
	2, 69, 272, 3, 2, 2, 2, 71, 274, 3, 2, 2, 2, 73, 283, 3, 2, 2, 2, 75, 294,
	3, 2, 2, 2, 77, 297, 3, 2, 2, 2, 79, 308, 3, 2, 2, 2, 81, 310, 3, 2, 2,
	2, 83, 312, 3, 2, 2, 2, 85, 314, 3, 2, 2, 2, 87, 321, 3, 2, 2, 2, 89, 328,
	3, 2, 2, 2, 91, 335, 3, 2, 2, 2, 93, 339, 3, 2, 2, 2, 95, 341, 3, 2, 2,
	2, 97, 343, 3, 2, 2, 2, 99, 345, 3, 2, 2, 2, 101, 360, 3, 2, 2, 2, 103,
	369, 3, 2, 2, 2, 105, 371, 3, 2, 2, 2, 107, 387, 3, 2, 2, 2, 109, 389,
	3, 2, 2, 2, 111, 396, 3, 2, 2, 2, 113, 408, 3, 2, 2, 2, 115, 411, 3, 2,
	2, 2, 117, 415, 3, 2, 2, 2, 119, 436, 3, 2, 2, 2, 121, 439, 3, 2, 2, 2,
	123, 450, 3, 2, 2, 2, 125, 126, 7, 42, 2, 2, 126, 4, 3, 2, 2, 2, 127, 128,
	7, 43, 2, 2, 128, 6, 3, 2, 2, 2, 129, 130, 7, 93, 2, 2, 130, 8, 3, 2, 2,
	2, 131, 132, 7, 46, 2, 2, 132, 10, 3, 2, 2, 2, 133, 134, 7, 95, 2, 2, 134,
	12, 3, 2, 2, 2, 135, 136, 7, 62, 2, 2, 136, 14, 3, 2, 2, 2, 137, 138, 7,
	62, 2, 2, 138, 139, 7, 63, 2, 2, 139, 16, 3, 2, 2, 2, 140, 141, 7, 64,
	2, 2, 141, 18, 3, 2, 2, 2, 142, 143, 7, 64, 2, 2, 143, 144, 7, 63, 2, 2,
	144, 20, 3, 2, 2, 2, 145, 146, 7, 63, 2, 2, 146, 147, 7, 63, 2, 2, 147,
	22, 3, 2, 2, 2, 148, 149, 7, 35, 2, 2, 149, 150, 7, 63, 2, 2, 150, 24,
	3, 2, 2, 2, 151, 152, 7, 110, 2, 2, 152, 153, 7, 107, 2, 2, 153, 154, 7,
	109, 2, 2, 154, 160, 7, 103, 2, 2, 155, 156, 7, 78, 2, 2, 156, 157, 7,
	75, 2, 2, 157, 158, 7, 77, 2, 2, 158, 160, 7, 71, 2, 2, 159, 151, 3, 2,
	2, 2, 159, 155, 3, 2, 2, 2, 160, 26, 3, 2, 2, 2, 161, 162, 7, 119, 2, 2,
	162, 163, 7, 102, 2, 2, 163, 168, 7, 104, 2, 2, 164, 165, 7, 87, 2, 2,
	165, 166, 7, 70, 2, 2, 166, 168, 7, 72, 2, 2, 167, 161, 3, 2, 2, 2, 167,
	164, 3, 2, 2, 2, 168, 28, 3, 2, 2, 2, 169, 170, 7, 45, 2, 2, 170, 30, 3,
	2, 2, 2, 171, 172, 7, 47, 2, 2, 172, 32, 3, 2, 2, 2, 173, 174, 7, 44, 2,
	2, 174, 34, 3, 2, 2, 2, 175, 176, 7, 49, 2, 2, 176, 36, 3, 2, 2, 2, 177,
	178, 7, 39, 2, 2, 178, 38, 3, 2, 2, 2, 179, 180, 7, 44, 2, 2, 180, 181,
	7, 44, 2, 2, 181, 40, 3, 2, 2, 2, 182, 183, 7, 62, 2, 2, 183, 184, 7, 62,
	2, 2, 184, 42, 3, 2, 2, 2, 185, 186, 7, 64, 2, 2, 186, 187, 7, 64, 2, 2,
	187, 44, 3, 2, 2, 2, 188, 189, 7, 40, 2, 2, 189, 46, 3, 2, 2, 2, 190, 191,
	7, 126, 2, 2, 191, 48, 3, 2, 2, 2, 192, 193, 7, 96, 2, 2, 193, 50, 3, 2,
	2, 2, 194, 195, 7, 40, 2, 2, 195, 200, 7, 40, 2, 2, 196, 197, 7, 99, 2,
	2, 197, 198, 7, 112, 2, 2, 198, 200, 7, 102, 2, 2, 199, 194, 3, 2, 2, 2,
	199, 196, 3, 2, 2, 2, 200, 52, 3, 2, 2, 2, 201, 202, 7, 126, 2, 2, 202,
	206, 7, 126, 2, 2, 203, 204, 7, 113, 2, 2, 204, 206, 7, 116, 2, 2, 205,
	201, 3, 2, 2, 2, 205, 203, 3, 2, 2, 2, 206, 54, 3, 2, 2, 2, 207, 208, 7,
	128, 2, 2, 208, 56, 3, 2, 2, 2, 209, 214, 7, 35, 2, 2, 210, 211, 7, 112,
	2, 2, 211, 212, 7, 113, 2, 2, 212, 214, 7, 118, 2, 2, 213, 209, 3, 2, 2,
	2, 213, 210, 3, 2, 2, 2, 214, 58, 3, 2, 2, 2, 215, 216, 7, 107, 2, 2, 216,
	217, 7, 112, 2, 2, 217, 60, 3, 2, 2, 2, 218, 219, 7, 112, 2, 2, 219, 220,
	7, 113, 2, 2, 220, 221, 7, 118, 2, 2, 221, 222, 7, 34, 2, 2, 222, 223,
	7, 107, 2, 2, 223, 224, 7, 112, 2, 2, 224, 62, 3, 2, 2, 2, 225, 230, 7,
	93, 2, 2, 226, 229, 5, 121, 61, 2, 227, 229, 5, 123, 62, 2, 228, 226, 3,
	2, 2, 2, 228, 227, 3, 2, 2, 2, 229, 232, 3, 2, 2, 2, 230, 228, 3, 2, 2,
	2, 230, 231, 3, 2, 2, 2, 231, 233, 3, 2, 2, 2, 232, 230, 3, 2, 2, 2, 233,
	234, 7, 95, 2, 2, 234, 64, 3, 2, 2, 2, 235, 236, 7, 118, 2, 2, 236, 237,
	7, 116, 2, 2, 237, 238, 7, 119, 2, 2, 238, 263, 7, 103, 2, 2, 239, 240,
	7, 86, 2, 2, 240, 241, 7, 116, 2, 2, 241, 242, 7, 119, 2, 2, 242, 263,
	7, 103, 2, 2, 243, 244, 7, 86, 2, 2, 244, 245, 7, 84, 2, 2, 245, 246, 7,
	87, 2, 2, 246, 263, 7, 71, 2, 2, 247, 248, 7, 104, 2, 2, 248, 249, 7, 99,
	2, 2, 249, 250, 7, 110, 2, 2, 250, 251, 7, 117, 2, 2, 251, 263, 7, 103,
	2, 2, 252, 253, 7, 72, 2, 2, 253, 254, 7, 99, 2, 2, 254, 255, 7, 110, 2,
	2, 255, 256, 7, 117, 2, 2, 256, 263, 7, 103, 2, 2, 257, 258, 7, 72, 2,
	2, 258, 259, 7, 67, 2, 2, 259, 260, 7, 78, 2, 2, 260, 261, 7, 85, 2, 2,
	261, 263, 7, 71, 2, 2, 262, 235, 3, 2, 2, 2, 262, 239, 3, 2, 2, 2, 262,
	243, 3, 2, 2, 2, 262, 247, 3, 2, 2, 2, 262, 252, 3, 2, 2, 2, 262, 257,
	3, 2, 2, 2, 263, 66, 3, 2, 2, 2, 264, 269, 5, 87, 44, 2, 265, 269, 5, 89,
	45, 2, 266, 269, 5, 91, 46, 2, 267, 269, 5, 85, 43, 2, 268, 264, 3, 2,
	2, 2, 268, 265, 3, 2, 2, 2, 268, 266, 3, 2, 2, 2, 268, 267, 3, 2, 2, 2,
	269, 68, 3, 2, 2, 2, 270, 273, 5, 103, 52, 2, 271, 273, 5, 105, 53, 2,
	272, 270, 3, 2, 2, 2, 272, 271, 3, 2, 2, 2, 273, 70, 3, 2, 2, 2, 274, 279,
	5, 81, 41, 2, 275, 278, 5, 81, 41, 2, 276, 278, 5, 83, 42, 2, 277, 275,
	3, 2, 2, 2, 277, 276, 3, 2, 2, 2, 278, 281, 3, 2, 2, 2, 279, 277, 3, 2,
	2, 2, 279, 280, 3, 2, 2, 2, 280, 72, 3, 2, 2, 2, 281, 279, 3, 2, 2, 2,
	282, 284, 5, 75, 38, 2, 283, 282, 3, 2, 2, 2, 283, 284, 3, 2, 2, 2, 284,
	285, 3, 2, 2, 2, 285, 287, 7, 36, 2, 2, 286, 288, 5, 77, 39, 2, 287, 286,
	3, 2, 2, 2, 287, 288, 3, 2, 2, 2, 288, 289, 3, 2, 2, 2, 289, 290, 7, 36,
	2, 2, 290, 74, 3, 2, 2, 2, 291, 292, 7, 119, 2, 2, 292, 295, 7, 58, 2,
	2, 293, 295, 9, 2, 2, 2, 294, 291, 3, 2, 2, 2, 294, 293, 3, 2, 2, 2, 295,
	76, 3, 2, 2, 2, 296, 298, 5, 79, 40, 2, 297, 296, 3, 2, 2, 2, 298, 299,
	3, 2, 2, 2, 299, 297, 3, 2, 2, 2, 299, 300, 3, 2, 2, 2, 300, 78, 3, 2,
	2, 2, 301, 309, 10, 3, 2, 2, 302, 309, 5, 119, 60, 2, 303, 304, 7, 94,
	2, 2, 304, 309, 7, 12, 2, 2, 305, 306, 7, 94, 2, 2, 306, 307, 7, 15, 2,
	2, 307, 309, 7, 12, 2, 2, 308, 301, 3, 2, 2, 2, 308, 302, 3, 2, 2, 2, 308,
	303, 3, 2, 2, 2, 308, 305, 3, 2, 2, 2, 309, 80, 3, 2, 2, 2, 310, 311, 9,
	4, 2, 2, 311, 82, 3, 2, 2, 2, 312, 313, 9, 5, 2, 2, 313, 84, 3, 2, 2, 2,
	314, 315, 7, 50, 2, 2, 315, 317, 9, 6, 2, 2, 316, 318, 9, 7, 2, 2, 317,
	316, 3, 2, 2, 2, 318, 319, 3, 2, 2, 2, 319, 317, 3, 2, 2, 2, 319, 320,
	3, 2, 2, 2, 320, 86, 3, 2, 2, 2, 321, 325, 5, 93, 47, 2, 322, 324, 5, 83,
	42, 2, 323, 322, 3, 2, 2, 2, 324, 327, 3, 2, 2, 2, 325, 323, 3, 2, 2, 2,
	325, 326, 3, 2, 2, 2, 326, 88, 3, 2, 2, 2, 327, 325, 3, 2, 2, 2, 328, 332,
	7, 50, 2, 2, 329, 331, 5, 95, 48, 2, 330, 329, 3, 2, 2, 2, 331, 334, 3,
	2, 2, 2, 332, 330, 3, 2, 2, 2, 332, 333, 3, 2, 2, 2, 333, 90, 3, 2, 2,
	2, 334, 332, 3, 2, 2, 2, 335, 336, 7, 50, 2, 2, 336, 337, 9, 8, 2, 2, 337,
	338, 5, 115, 58, 2, 338, 92, 3, 2, 2, 2, 339, 340, 9, 9, 2, 2, 340, 94,
	3, 2, 2, 2, 341, 342, 9, 10, 2, 2, 342, 96, 3, 2, 2, 2, 343, 344, 9, 11,
	2, 2, 344, 98, 3, 2, 2, 2, 345, 346, 5, 97, 49, 2, 346, 347, 5, 97, 49,
	2, 347, 348, 5, 97, 49, 2, 348, 349, 5, 97, 49, 2, 349, 100, 3, 2, 2, 2,
	350, 351, 7, 94, 2, 2, 351, 352, 7, 119, 2, 2, 352, 353, 3, 2, 2, 2, 353,
	361, 5, 99, 50, 2, 354, 355, 7, 94, 2, 2, 355, 356, 7, 87, 2, 2, 356, 357,
	3, 2, 2, 2, 357, 358, 5, 99, 50, 2, 358, 359, 5, 99, 50, 2, 359, 361, 3,
	2, 2, 2, 360, 350, 3, 2, 2, 2, 360, 354, 3, 2, 2, 2, 361, 102, 3, 2, 2,
	2, 362, 364, 5, 107, 54, 2, 363, 365, 5, 109, 55, 2, 364, 363, 3, 2, 2,
	2, 364, 365, 3, 2, 2, 2, 365, 370, 3, 2, 2, 2, 366, 367, 5, 111, 56, 2,
	367, 368, 5, 109, 55, 2, 368, 370, 3, 2, 2, 2, 369, 362, 3, 2, 2, 2, 369,
	366, 3, 2, 2, 2, 370, 104, 3, 2, 2, 2, 371, 372, 7, 50, 2, 2, 372, 375,
	9, 8, 2, 2, 373, 376, 5, 113, 57, 2, 374, 376, 5, 115, 58, 2, 375, 373,
	3, 2, 2, 2, 375, 374, 3, 2, 2, 2, 376, 377, 3, 2, 2, 2, 377, 378, 5, 117,
	59, 2, 378, 106, 3, 2, 2, 2, 379, 381, 5, 111, 56, 2, 380, 379, 3, 2, 2,
	2, 380, 381, 3, 2, 2, 2, 381, 382, 3, 2, 2, 2, 382, 383, 7, 48, 2, 2, 383,
	388, 5, 111, 56, 2, 384, 385, 5, 111, 56, 2, 385, 386, 7, 48, 2, 2, 386,
	388, 3, 2, 2, 2, 387, 380, 3, 2, 2, 2, 387, 384, 3, 2, 2, 2, 388, 108,
	3, 2, 2, 2, 389, 391, 9, 12, 2, 2, 390, 392, 9, 13, 2, 2, 391, 390, 3,
	2, 2, 2, 391, 392, 3, 2, 2, 2, 392, 393, 3, 2, 2, 2, 393, 394, 5, 111,
	56, 2, 394, 110, 3, 2, 2, 2, 395, 397, 5, 83, 42, 2, 396, 395, 3, 2, 2,
	2, 397, 398, 3, 2, 2, 2, 398, 396, 3, 2, 2, 2, 398, 399, 3, 2, 2, 2, 399,
	112, 3, 2, 2, 2, 400, 402, 5, 115, 58, 2, 401, 400, 3, 2, 2, 2, 401, 402,
	3, 2, 2, 2, 402, 403, 3, 2, 2, 2, 403, 404, 7, 48, 2, 2, 404, 409, 5, 115,
	58, 2, 405, 406, 5, 115, 58, 2, 406, 407, 7, 48, 2, 2, 407, 409, 3, 2,
	2, 2, 408, 401, 3, 2, 2, 2, 408, 405, 3, 2, 2, 2, 409, 114, 3, 2, 2, 2,
	410, 412, 5, 97, 49, 2, 411, 410, 3, 2, 2, 2, 412, 413, 3, 2, 2, 2, 413,
	411, 3, 2, 2, 2, 413, 414, 3, 2, 2, 2, 414, 116, 3, 2, 2, 2, 415, 417,
	9, 14, 2, 2, 416, 418, 9, 13, 2, 2, 417, 416, 3, 2, 2, 2, 417, 418, 3,
	2, 2, 2, 418, 419, 3, 2, 2, 2, 419, 420, 5, 111, 56, 2, 420, 118, 3, 2,
	2, 2, 421, 422, 7, 94, 2, 2, 422, 437, 9, 15, 2, 2, 423, 424, 7, 94, 2,
	2, 424, 426, 5, 95, 48, 2, 425, 427, 5, 95, 48, 2, 426, 425, 3, 2, 2, 2,
	426, 427, 3, 2, 2, 2, 427, 429, 3, 2, 2, 2, 428, 430, 5, 95, 48, 2, 429,
	428, 3, 2, 2, 2, 429, 430, 3, 2, 2, 2, 430, 437, 3, 2, 2, 2, 431, 432,
	7, 94, 2, 2, 432, 433, 7, 122, 2, 2, 433, 434, 3, 2, 2, 2, 434, 437, 5,
	115, 58, 2, 435, 437, 5, 101, 51, 2, 436, 421, 3, 2, 2, 2, 436, 423, 3,
	2, 2, 2, 436, 431, 3, 2, 2, 2, 436, 435, 3, 2, 2, 2, 437, 120, 3, 2, 2,
	2, 438, 440, 9, 16, 2, 2, 439, 438, 3, 2, 2, 2, 440, 441, 3, 2, 2, 2, 441,
	439, 3, 2, 2, 2, 441, 442, 3, 2, 2, 2, 442, 443, 3, 2, 2, 2, 443, 444,
	8, 61, 2, 2, 444, 122, 3, 2, 2, 2, 445, 447, 7, 15, 2, 2, 446, 448, 7,
	12, 2, 2, 447, 446, 3, 2, 2, 2, 447, 448, 3, 2, 2, 2, 448, 451, 3, 2, 2,
	2, 449, 451, 7, 12, 2, 2, 450, 445, 3, 2, 2, 2, 450, 449, 3, 2, 2, 2, 451,
	452, 3, 2, 2, 2, 452, 453, 8, 62, 2, 2, 453, 124, 3, 2, 2, 2, 41, 2, 159,
	167, 199, 205, 213, 228, 230, 262, 268, 272, 277, 279, 283, 287, 294, 299,
	308, 319, 325, 332, 360, 364, 369, 375, 380, 387, 391, 398, 401, 408, 413,
	417, 426, 429, 436, 441, 447, 450, 3, 8, 2, 2,
}

var lexerChannelNames = []string{
	"DEFAULT_TOKEN_CHANNEL", "HIDDEN",
}

var lexerModeNames = []string{
	"DEFAULT_MODE",
}

var lexerLiteralNames = []string{
	"", "'('", "')'", "'['", "','", "']'", "'<'", "'<='", "'>'", "'>='", "'=='",
	"'!='", "", "", "'+'", "'-'", "'*'", "'/'", "'%'", "'**'", "'<<'", "'>>'",
	"'&'", "'|'", "'^'", "", "", "'~'", "", "'in'", "'not in'",
}

var lexerSymbolicNames = []string{
	"", "", "", "", "", "", "LT", "LE", "GT", "GE", "EQ", "NE", "LIKE", "UDF",
	"ADD", "SUB", "MUL", "DIV", "MOD", "POW", "SHL", "SHR", "BAND", "BOR",
	"BXOR", "AND", "OR", "BNOT", "NOT", "IN", "NIN", "EmptyTerm", "BooleanConstant",
	"IntegerConstant", "FloatingConstant", "Identifier", "StringLiteral", "Whitespace",
	"Newline",
}

var lexerRuleNames = []string{
	"T__0", "T__1", "T__2", "T__3", "T__4", "LT", "LE", "GT", "GE", "EQ", "NE",
	"LIKE", "UDF", "ADD", "SUB", "MUL", "DIV", "MOD", "POW", "SHL", "SHR",
	"BAND", "BOR", "BXOR", "AND", "OR", "BNOT", "NOT", "IN", "NIN", "EmptyTerm",
	"BooleanConstant", "IntegerConstant", "FloatingConstant", "Identifier",
	"StringLiteral", "EncodingPrefix", "SCharSequence", "SChar", "Nondigit",
	"Digit", "BinaryConstant", "DecimalConstant", "OctalConstant", "HexadecimalConstant",
	"NonzeroDigit", "OctalDigit", "HexadecimalDigit", "HexQuad", "UniversalCharacterName",
	"DecimalFloatingConstant", "HexadecimalFloatingConstant", "FractionalConstant",
	"ExponentPart", "DigitSequence", "HexadecimalFractionalConstant", "HexadecimalDigitSequence",
	"BinaryExponentPart", "EscapeSequence", "Whitespace", "Newline",
}

type PlanLexer struct {
	*antlr.BaseLexer
	channelNames []string
	modeNames    []string
	// TODO: EOF string
}

// NewPlanLexer produces a new lexer instance for the optional input antlr.CharStream.
//
// The *PlanLexer instance produced may be reused by calling the SetInputStream method.
// The initial lexer configuration is expensive to construct, and the object is not thread-safe;
// however, if used within a Golang sync.Pool, the construction cost amortizes well and the
// objects can be used in a thread-safe manner.
func NewPlanLexer(input antlr.CharStream) *PlanLexer {
	l := new(PlanLexer)
	lexerDeserializer := antlr.NewATNDeserializer(nil)
	lexerAtn := lexerDeserializer.DeserializeFromUInt16(serializedLexerAtn)
	lexerDecisionToDFA := make([]*antlr.DFA, len(lexerAtn.DecisionToState))
	for index, ds := range lexerAtn.DecisionToState {
		lexerDecisionToDFA[index] = antlr.NewDFA(ds, index)
	}
	l.BaseLexer = antlr.NewBaseLexer(input)
	l.Interpreter = antlr.NewLexerATNSimulator(l, lexerAtn, lexerDecisionToDFA, antlr.NewPredictionContextCache())

	l.channelNames = lexerChannelNames
	l.modeNames = lexerModeNames
	l.RuleNames = lexerRuleNames
	l.LiteralNames = lexerLiteralNames
	l.SymbolicNames = lexerSymbolicNames
	l.GrammarFileName = "Plan.g4"
	// TODO: l.EOF = antlr.TokenEOF

	return l
}

// PlanLexer tokens.
const (
	PlanLexerT__0             = 1
	PlanLexerT__1             = 2
	PlanLexerT__2             = 3
	PlanLexerT__3             = 4
	PlanLexerT__4             = 5
	PlanLexerLT               = 6
	PlanLexerLE               = 7
	PlanLexerGT               = 8
	PlanLexerGE               = 9
	PlanLexerEQ               = 10
	PlanLexerNE               = 11
	PlanLexerLIKE             = 12
	PlanLexerUDF              = 13
	PlanLexerADD              = 14
	PlanLexerSUB              = 15
	PlanLexerMUL              = 16
	PlanLexerDIV              = 17
	PlanLexerMOD              = 18
	PlanLexerPOW              = 19
	PlanLexerSHL              = 20
	PlanLexerSHR              = 21
	PlanLexerBAND             = 22
	PlanLexerBOR              = 23
	PlanLexerBXOR             = 24
	PlanLexerAND              = 25
	PlanLexerOR               = 26
	PlanLexerBNOT             = 27
	PlanLexerNOT              = 28
	PlanLexerIN               = 29
	PlanLexerNIN              = 30
	PlanLexerEmptyTerm        = 31
	PlanLexerBooleanConstant  = 32
	PlanLexerIntegerConstant  = 33
	PlanLexerFloatingConstant = 34
	PlanLexerIdentifier       = 35
	PlanLexerStringLiteral    = 36
	PlanLexerWhitespace       = 37
	PlanLexerNewline          = 38
)
