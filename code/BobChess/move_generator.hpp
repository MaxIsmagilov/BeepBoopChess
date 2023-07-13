#pragma once

#include "board.hpp"
#include "move_list.hpp"
#include "utils.hpp"

namespace BobChess
{

class MoveGenerator
{
 public:
  /// @brief generate a list of moves
  /// @param bd
  /// @return a MoveList object
  static MoveList generate_all(const Board& bd) noexcept;

  /// @brief initialize all
  static void initialize_all() noexcept;

  /// @brief checks if a square is attacked
  /// @param square
  /// @param bd
  /// @param attacking_side
  /// @return whether the square is attacked or not
  static bool is_attacked(int square, const Board& bd, const int attacking_side);

  /// @brief checks if a side is in check
  /// @param bd
  /// @param side_in_check
  /// @return whether the side is in check
  static bool in_check(const Board& bd, const int side_in_check);

 private:
  /// @brief the bits for the a-file
  static constexpr u64 A_FILE = 72340172838076673ULL;

  /// @brief the bits for the b-file
  static constexpr u64 B_FILE = 144680345676153346ULL;

  /// @brief the bits for the g-file
  static constexpr u64 G_FILE = 4629771061636907072ULL;

  /// @brief the bits for the h-file
  static constexpr u64 H_FILE = 9259542123273814144ULL;

  /// @brief utility enum for magics (ROOK, BISHOP)
  enum rook_or_bishop { ROOK_M, BISHOP_M };

  /// @brief pawn attack table
  /// @note 0 is black
  static u64 pawn_attacks[2][64];
  /// @brief knight attack table
  static u64 knight_attacks[64];
  /// @brief king attack table
  static u64 king_attacks[64];

  /// @brief pawn silent move table
  /// @note 0 is black
  static u64 silent_pawn_moves[2][64];  //

  /// @brief occupancy masks for bishops
  static u64 bishop_attack_masks[64];
  /// @brief occupancy masks for rooks
  static u64 rook_attack_masks[64];

  /// @brief bishop attack table
  static u64 magic_bishop_attacks[64][512];
  /// @brief rook attack table
  static u64 magic_rook_attacks[64][4096];

  /// @brief occupancy bits for a square for a bishop
  static constexpr int bishop_occupancy_bits[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
                                                    5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
                                                    7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};
  /// @brief occupancy bits for a square for a rook
  static constexpr int rook_occupancy_bits[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
                                                  11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
                                                  11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
                                                  11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

  /// @brief find an attack mask for a pawn
  /// @param square
  /// @param side 1 for white, -1 for black
  /// @return a bitboard mask
  static u64 pawn_mask(int square, int side);

  /// @brief find a mask for a knight
  /// @param square
  /// @return a bitboard mask
  static u64 knight_mask(int square);

  /// @brief find a mask for a king
  /// @param square
  /// @return a bitboard mask
  static u64 king_mask(int square);

  /// @brief find a mask for bishop occupancies
  /// @param square
  /// @return a bitboard mask
  static u64 bishop_mask(int square);

  /// @brief find a mask for rook occupancies
  /// @param square
  /// @return a bitboard mask
  static u64 rook_mask(int square);

  /// @brief silent masks for pawns
  /// @param square
  /// @param side
  /// @return a bitboard mask
  static u64 silent_pawn_mask(int square, int side);

  /// @brief on the fly attacks for bishops
  /// @param square
  /// @param blockers
  /// @return an attack bitboard
  /// @note for magic generation
  static u64 otf_bishop_attacks(int square, u64 blockers);

  /// @brief on the fly attacks for rooks
  /// @param square
  /// @param blockers
  /// @return an attack bitboard
  /// @note for magic generation
  static u64 otf_rook_attacks(int square, u64 blockers);

  /// @brief gets the occupancy for a mask
  /// @param index
  /// @param bit_count
  /// @param mask
  /// @return a bitboard
  /// @note for magic generation
  static u64 get_occupancy(int index, int bit_count, u64 mask);

  /// @brief initialize attack tables for non-slider pieces (pawns, knights, kings)
  static void initialize_non_sliders();

  /// @brief initialize attack tables for slider pieces (bishops, rooks, queens)
  static void initialize_sliders();

  /// @brief gets bishop attacks for an occupancy
  /// @param square
  /// @param occupancy
  /// @return an attack bitboard
  static u64 get_bishop_attacks(const int square, u64 occupancy);

  /// @brief gets rook attacks for an occupancy
  /// @param square
  /// @param occupancy
  /// @return an attack bitboard
  static u64 get_rook_attacks(const int square, u64 occupancy);

  /// @brief gets queen attacks for an occupancy
  /// @param square
  /// @param occupancy
  /// @return an attack bitboard
  static u64 get_queen_attacks(const int square, u64 occupancy);

  /// @brief checks whether a move would put the moving side in check
  /// @param bd
  /// @param move
  /// @return boolean value
  static bool check_check(const Board& bd, const Move& move);
};

}  // namespace BobChess
