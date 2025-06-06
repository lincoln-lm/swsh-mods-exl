#pragma once

#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "savefile.hpp"
#include <random>

class MersenneTwister : public std::mt19937_64 {
    public:
        using std::mt19937_64::mt19937_64;
        u64 RandMax(u64 maximum) {
            if (maximum == 0 || maximum == 1) {
                return 0;
            }
            u64 mask = std::bit_ceil(maximum) - 1;
            u64 result;
            do {
                result = this->operator()() & mask;
            } while (result >= maximum);
            return result;
        }
        u64 RandRange(u64 minimum, u64 maximum) {
            u64 range = maximum - minimum + 1;
            return minimum + this->RandMax(range);
        }
        template<typename T, std::size_t Extent>
        T RandElement(const std::span<const T, Extent> input) {
            return input[this->RandMax(input.size())];
        }
        f64 RandDouble() {
            return static_cast<f64>(this->operator()() / std::numeric_limits<u64>::max());
        }
        bool RandChance(u64 denominator) {
            return this->RandMax(denominator) == 0;
        }
        std::tuple<u32, u16> RandSpeciesAndForm() {
            u32 species;
            u16 form;
            do {
                species = this->RandRange(1, 899);
                PersonalInfo::FetchInfo(species, 0);
                u32 form_count = PersonalInfo::GetField(PersonalInfo::InfoField::FORM_COUNT);
                form = this->RandMax(form_count);
            } while (!PersonalInfo::isInGame(species, form));
            return {species, form};
        }
        s16 RandValidMoveId() {
            s16 move_id;
            do {
                move_id = static_cast<s16>(this->RandRange(1, 820));
            } while (!MoveIdHolder{0, move_id}.IsMoveUsable());
            return move_id;
        }
        void RandMoves(std::span<s16, 4> moves) {
            for (size_t i = 0; i < moves.size(); ++i) {
                do {
                    moves[i] = this->RandValidMoveId();
                } while (std::find(moves.begin(), moves.begin() + i, moves[i]) != moves.begin() + i);
            }
        }
        std::array<s16, 4> RandMoves() {
            std::array<s16, 4> moves;
            this->RandMoves(moves);
            return moves;
        }
        u16 RandHeldItem() {
            return this->RandElement(std::span(VALID_HELD_ITEMS));
        }
        u16 RandTM() {
            return this->RandElement(std::span(VALID_TMS));
        }
        u16 RandAbility() {
            return this->RandRange(1, 268);
        }
        u16 RandBall() {
            return this->RandRange(1, 27);
        }
};

namespace RngManager {
    template<typename T, size_t Size>
    requires exl::util::impl::murmur3::Hashable<T>
    MersenneTwister NewRandomGenerator(std::span<const T, Size> input) {
        return MersenneTwister(exl::util::Murmur3::Compute(input, save_file.rng_seed));
    }
    MersenneTwister NewRandomGenerator(const std::string input) {
        return NewRandomGenerator(std::span(input));
    }
    template<typename T>
    requires std::is_integral_v<T>
    MersenneTwister NewRandomGenerator(const T input) {
        return NewRandomGenerator(std::span<const char, sizeof(T)>(reinterpret_cast<const char*>(&input), sizeof(T)));
    }
    MersenneTwister NewRandomGenerator() {
        return NewRandomGenerator(exl::util::GetRandomU64());
    }
};