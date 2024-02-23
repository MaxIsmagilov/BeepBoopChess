#include <algorithm>
#include <iostream>

#include "magics.hpp"
#include "move_generator.hpp"

namespace BeepBoop
{

MoveList MoveGenerator::generate_all(const Board& bd) noexcept {
  MoveList movelist;

  // initiallize constants
  const u64  all_pieces    = bd.all_occ();
  const bool side          = bd.side_to_move();
  const u64  attacking_occ = (side) ? bd.white_occ() : bd.black_occ();
  const u64  defending_occ = (side) ? bd.black_occ() : bd.white_occ();

  // set pointers for attacking and defending pieces
  const int attacking_pieces = (!side) * 6;

  // generate pawn moves
  u64 piece_board = bd[utils::PAWN + attacking_pieces];
  u64 att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    u32 ep_square    = bd.enpassant_square();
    u64 t_enpassant  = (ep_square == 255) ? 0ULL : (1ULL << ep_square);
    att |= (pawn_attacks[side][square] & (defending_occ | t_enpassant));
    att &= ~attacking_occ;
    if (side && !((1ULL << (square - 8)) & all_pieces)) {
      att |= (silent_pawn_moves[side][square] & ~all_pieces);
    } else if ((!side) && !((1ULL << (square + 8)) & all_pieces)) {
      att |= (silent_pawn_moves[side][square] & ~all_pieces);
    }

    while (att) {
      const int j             = utils::LSB_index(att);
      att                     = utils::set_bit_false(att, j);
      const bool pp_flag      = (abs(square - j) == 16) ? 0b1 : 0b0;
      const bool captureflg   = ((1ULL << j) & defending_occ);
      const bool enpassantflg = (static_cast<decltype(bd.enpassant_square())>(j) == bd.enpassant_square());

      Move move(square, j, utils::PAWN, utils::PAWN, captureflg || enpassantflg, 0, 0, enpassantflg, pp_flag);

      // check for promotions
      if ((j / 8 == 0) || (j / 8 == 7)) {
        for (u32 promotee = utils::KNIGHT; promotee < utils::KING; ++promotee) {
          move = Move(square, j, utils::PAWN, promotee, captureflg, 0, 1, 0, pp_flag);
          movelist.push_back(move);
        }
      } else {
        movelist.push_back(move);
      }
    }
  }

  // generate knight moves
  piece_board = bd[utils::KNIGHT + attacking_pieces];
  att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    att |= knight_attacks[square];
    att &= ~attacking_occ;
    while (att) {
      const int j     = utils::LSB_index(att);
      att             = utils::set_bit_false(att, j);
      bool captureflg = ((1ULL << j) & defending_occ);
      Move move(square, j, utils::KNIGHT, 0, captureflg, 0, 0, 0, 0);

      movelist.push_back(move);
    }
  }

  // generate bishop moves
  piece_board = bd[utils::BISHOP + attacking_pieces];
  att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    att |= get_bishop_attacks(square, all_pieces);
    att &= ~attacking_occ;
    while (att) {
      const int j     = utils::LSB_index(att);
      att             = utils::set_bit_false(att, j);
      bool captureflg = ((1ULL << j) & defending_occ);
      Move move(square, j, utils::BISHOP, 0, captureflg, 0, 0, 0, 0);

      movelist.push_back(move);
    }
  }

  // generate rook moves
  piece_board = bd[utils::ROOK + attacking_pieces];
  att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    att |= get_rook_attacks(square, all_pieces);
    att &= ~attacking_occ;

    while (att) {
      const int j     = utils::LSB_index(att);
      att             = utils::set_bit_false(att, j);
      bool captureflg = ((1ULL << j) & defending_occ);
      Move move(square, j, utils::ROOK, 0, captureflg, 0, 0, 0, 0);

      movelist.push_back(move);
    }
  }

  // generate queen moves
  piece_board = bd[utils::QUEEN + attacking_pieces];
  att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    att |= get_queen_attacks(square, all_pieces);
    att &= ~attacking_occ;
    while (att) {
      const int j     = utils::LSB_index(att);
      att             = utils::set_bit_false(att, j);
      bool captureflg = ((1ULL << j) & defending_occ);
      Move move(square, j, utils::QUEEN, 0, captureflg, 0, 0, 0, 0);

      movelist.push_back(move);
    }
  }

  // generate king moves
  piece_board = bd[utils::KING + attacking_pieces];
  att         = 0ULL;
  while (piece_board) {
    const int square = utils::LSB_index(piece_board);
    piece_board      = utils::set_bit_false(piece_board, square);
    att |= king_attacks[square];
    att &= ~attacking_occ;

    while (att) {
      const int j     = utils::LSB_index(att);
      att             = utils::set_bit_false(att, j);
      bool captureflg = ((1ULL << j) & defending_occ);
      Move move(square, j, utils::KING, 0, captureflg, 0, 0, 0, 0);

      movelist.push_back(move);
    }
  }

  // castling routine
  if (side) {
    if (bd.castle_available(Board::WHITE_SHORT)) {
      if (!(is_attacked(utils::_E1, bd, 0) || is_attacked(utils::_F1, bd, 0) || is_attacked(utils::_G1, bd, 0)) &&
          !(all_pieces & ((1ULL << utils::_F1) | (1ULL << utils::_G1))) && (utils::LSB_index(bd[5]) == utils::_E1)) {
        movelist.push_back(Move(utils::_E1, utils::_G1, utils::KING, 0, 0, 1, 0, 0, 0));
      }
    }
    if (bd.castle_available(Board::WHITE_LONG)) {
      if (!(is_attacked(utils::_E1, bd, 0) || is_attacked(utils::_D1, bd, 0) || is_attacked(utils::_C1, bd, 0)) &&
          !(all_pieces & ((1ULL << utils::_D1) | (1ULL << utils::_C1) | (1ULL << utils::_B1))) &&
          (utils::LSB_index(bd[5]) == utils::_E1)) {
        movelist.push_back(Move(utils::_E1, utils::_C1, utils::KING, 0, 0, 1, 0, 0, 0));
      }
    }
  } else {
    if (bd.castle_available(Board::BLACK_SHORT)) {
      if (!(is_attacked(utils::_E8, bd, 1) || is_attacked(utils::_F8, bd, 1) || is_attacked(utils::_G8, bd, 1)) &&
          !(all_pieces & ((1ULL << utils::_F8) | (1ULL << utils::_G8))) && (utils::LSB_index(bd[11]) == utils::_E8)) {
        movelist.push_back(Move(utils::_E8, utils::_G8, utils::KING, 0, 0, 1, 0, 0, 0));
      }
    }
    if (bd.castle_available(Board::BLACK_LONG)) {
      if (!(is_attacked(utils::_E8, bd, 1) || is_attacked(utils::_D8, bd, 1) || is_attacked(utils::_C8, bd, 1)) &&
          !(all_pieces & ((1ULL << utils::_D8) | (1ULL << utils::_C8) | (1ULL << utils::_B8))) &&
          (utils::LSB_index(bd[11]) == utils::_E8)) {
        movelist.push_back(Move(utils::_E8, utils::_C8, utils::KING, 0, 0, 1, 0, 0, 0));
      }
    }
  }

  auto size = movelist.get_size();

  // exit if empty
  if (size == 0) return movelist;

  std::vector<int> v;

  for (int i = 0; i < movelist.get_size(); ++i) {
    if (check_check(bd, movelist[i])) {
      v.push_back(i);
    }
  }

  movelist.remove(v);

  return movelist;
}

void MoveGenerator::initialize_all() noexcept {
  initialize_sliders();
  initialize_non_sliders();
}

u64 MoveGenerator::pawn_mask(int square, int side) {
  u64 att      = 0ULL;  // attacks
  u64 bitboard = 0ULL;  // pawn square
  bitboard     = utils::set_bit_true(bitboard, square);
  if (side == 1)  // white's attacks
  {
    att |= ((bitboard & A_FILE) ? 0ULL : (bitboard >> 9)) | ((bitboard & H_FILE) ? 0ULL : (bitboard >> 7));
  } else  // black's attacks
  {
    att |= ((bitboard & A_FILE) ? 0ULL : (bitboard << 7)) | ((bitboard & H_FILE) ? 0ULL : (bitboard << 9));
  }
  return att;
}

u64 MoveGenerator::knight_mask(int square) {
  u64 att      = 0ULL;  // attacks
  u64 bitboard = 0ULL;  // knight square
  bitboard     = utils::set_bit_true(bitboard, square);
  // generate 17 and -15 attacks
  if (!(bitboard & A_FILE)) {
    att |= (bitboard >> 17) | (bitboard << 15);
  }
  // generate -17 and 15 attacks
  if (!(bitboard & H_FILE)) {
    att |= (bitboard << 17) | (bitboard >> 15);
  }
  // generate 10 and -6 attacks
  if (!((bitboard & A_FILE) || (bitboard & B_FILE))) {
    att |= (bitboard >> 10) | (bitboard << 6);
  }
  // generate -10 and 6 attacks
  if (!((bitboard & H_FILE) || (bitboard & G_FILE))) {
    att |= (bitboard << 10) | (bitboard >> 6);
  }
  return att;
}

u64 MoveGenerator::king_mask(int square) {
  u64 att      = 0ULL;  // attacks
  u64 bitboard = 0ULL;  // king square
  bitboard     = utils::set_bit_true(bitboard, square);
  if (!(bitboard & H_FILE)) {
    att |= (bitboard << 9) | (bitboard << 1) | (bitboard >> 7);
  }
  if (!(bitboard & A_FILE)) {
    att |= (bitboard >> 9) | (bitboard >> 1) | (bitboard << 7);
  }
  att |= (bitboard << 8) | (bitboard >> 8);
  return att;
}

u64 MoveGenerator::bishop_mask(int square) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  u64 att    = 0ULL;
  for (rank = t_rank + 1, file = t_file + 1; rank <= 6 && file <= 6; rank++, file++) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank - 1, file = t_file + 1; rank >= 1 && file <= 6; rank--, file++) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank + 1, file = t_file - 1; rank <= 6 && file >= 1; rank++, file--) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank - 1, file = t_file - 1; rank >= 1 && file >= 1; rank--, file--) att |= 1ULL << (rank * 8 + file);
  return att;
}

u64 MoveGenerator::rook_mask(int square) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  u64 att    = 0ULL;
  for (rank = t_rank + 1; rank <= 6; rank++) att |= (1ULL << (rank * 8 + t_file));
  for (rank = t_rank - 1; rank >= 1; rank--) att |= (1ULL << (rank * 8 + t_file));
  for (file = t_file + 1; file <= 6; file++) att |= (1ULL << (t_rank * 8 + file));
  for (file = t_file - 1; file >= 1; file--) att |= (1ULL << (t_rank * 8 + file));
  return att;
}

u64 MoveGenerator::silent_pawn_mask(int square, int side) {
  u64 mv       = 0ULL;
  u64 bitboard = 0ULL;  // pawn square
  bitboard     = utils::set_bit_true(bitboard, square);
  if (side == 1) {
    mv |= (bitboard >> 8);
    if (square / 8 == 6) mv |= (bitboard >> 16);
  } else {
    mv |= (bitboard << 8);
    if (square / 8 == 1) mv |= (bitboard << 16);
  }
  return mv;
}

u64 MoveGenerator::otf_bishop_attacks(int square, u64 blockers) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  u64 att    = 0ULL;
  for (rank = t_rank + 1, file = t_file + 1; rank <= 7 && file <= 7; rank++, file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file <= 7; rank--, file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank + 1, file = t_file - 1; rank <= 7 && file >= 0; rank++, file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0; rank--, file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  return att;
}

u64 MoveGenerator::otf_rook_attacks(int square, u64 blockers) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  u64 att    = 0ULL;
  for (rank = t_rank + 1, file = t_file; rank <= 7; rank++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file; rank >= 0; rank--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank, file = t_file + 1; file <= 7; file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank, file = t_file - 1; file >= 0; file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  return att;
}

u64 MoveGenerator::get_occupancy(int index, int bit_count, u64 mask) {
  u64 occupancy = 0ULL;

  for (int i = 0; i < bit_count; i++) {
    const int square = utils::LSB_index(mask);
    mask             = utils::set_bit_false(mask, square);
    if (index & (1ULL << i)) {
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}

void MoveGenerator::initialize_non_sliders() {
  for (int i = 0; i < 64; i++) {
    pawn_attacks[0][i]      = pawn_mask(i, 0);
    pawn_attacks[1][i]      = pawn_mask(i, 1);
    knight_attacks[i]       = knight_mask(i);
    king_attacks[i]         = king_mask(i);
    silent_pawn_moves[0][i] = silent_pawn_mask(i, 0);
    silent_pawn_moves[1][i] = silent_pawn_mask(i, 1);
    // std::cout << utils::print_bitboard(silent_pawn_moves[0][i] | (1ULL << i)) << '\n';
  }
}

void MoveGenerator::initialize_sliders() {
  for (int i = 0; i < 64; i++) {
    bishop_attack_masks[i] = bishop_mask(i);
    rook_attack_masks[i]   = rook_mask(i);

    u64 b_attack_mask = bishop_attack_masks[i];
    u64 r_attack_mask = rook_attack_masks[i];

    int b_bits = utils::count_bits(b_attack_mask);
    int r_bits = utils::count_bits(r_attack_mask);

    int b_occi = 1 << b_bits;
    int r_occi = 1 << r_bits;

    for (int index = 0; index < b_occi; index++) {
      u64 occ = get_occupancy(index, b_bits, b_attack_mask);

      int magic_index = (occ * bishop_magics[i]) >> (64 - bishop_occupancy_bits[i]);

      magic_bishop_attacks[i][magic_index] = otf_bishop_attacks(i, occ);
    }

    for (int index = 0; index < r_occi; index++) {
      u64 occ = get_occupancy(index, r_bits, r_attack_mask);

      int magic_index = (occ * rook_magics[i]) >> (64 - rook_occupancy_bits[i]);

      magic_rook_attacks[i][magic_index] = otf_rook_attacks(i, occ);
    }
  }
}

u64 MoveGenerator::get_bishop_attacks(const int square, u64 occupancy) {
  occupancy &= bishop_attack_masks[square];
  occupancy *= bishop_magics[square];
  occupancy >>= 64 - bishop_occupancy_bits[square];
  return magic_bishop_attacks[square][occupancy];
}

u64 MoveGenerator::get_rook_attacks(const int square, u64 occupancy) {
  occupancy &= rook_attack_masks[square];
  occupancy *= rook_magics[square];
  occupancy >>= 64 - rook_occupancy_bits[square];
  return magic_rook_attacks[square][occupancy];
}

u64 MoveGenerator::get_queen_attacks(const int square, u64 occupancy) {
  u64 occ1 = occupancy;

  occ1 &= bishop_attack_masks[square];
  occ1 *= bishop_magics[square];
  occ1 >>= 64 - bishop_occupancy_bits[square];

  u64 occ2 = occupancy;

  occ2 &= rook_attack_masks[square];
  occ2 *= rook_magics[square];
  occ2 >>= 64 - rook_occupancy_bits[square];
  return magic_rook_attacks[square][occ2] | magic_bishop_attacks[square][occ1];
}

bool MoveGenerator::is_attacked(int square, const Board& bd, const int attacking_side) {
  if (square == -1) {
    return true;
  } else {
    if (attacking_side == 1)
      return ((pawn_attacks[0][square] & bd[0]) || (knight_attacks[square] & bd[1]) ||
              (get_bishop_attacks(square, bd.all_occ()) & (bd[2] | bd[4])) ||
              (get_rook_attacks(square, bd.all_occ()) & (bd[3] | bd[4])) || (king_attacks[square] & bd[5]));
    else
      return ((pawn_attacks[1][square] & bd[6]) || (knight_attacks[square] & bd[7]) ||
              (get_bishop_attacks(square, bd.all_occ()) & (bd[8] | bd[10])) ||
              (get_rook_attacks(square, bd.all_occ()) & (bd[9] | bd[10])) || (king_attacks[square] & bd[11]));
  }
}

bool MoveGenerator::in_check(const Board& bd, const int side_in_check) {
  return ((side_in_check == 0) ? is_attacked(utils::LSB_index(bd[11]), bd, 1)
                               : is_attacked(utils::LSB_index(bd[5]), bd, 0));
}

bool MoveGenerator::check_check(const Board& bd, const Move& move) {
  return in_check(bd.move_copy(move), bd.side_to_move());
}

u64 MoveGenerator::pawn_attacks[2][64] = {0};

u64 MoveGenerator::knight_attacks[64] = {0};

u64 MoveGenerator::king_attacks[64] = {0};

u64 MoveGenerator::silent_pawn_moves[2][64] = {0};

u64 MoveGenerator::bishop_attack_masks[64] = {0};

u64 MoveGenerator::rook_attack_masks[64] = {0};

u64 MoveGenerator::magic_bishop_attacks[64][512] = {0};

u64 MoveGenerator::magic_rook_attacks[64][4096] = {0};

}  // namespace BeepBoop
