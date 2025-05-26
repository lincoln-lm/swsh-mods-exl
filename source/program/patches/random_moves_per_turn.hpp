

HOOK_DEFINE_INLINE(RandomMovesPerTurn) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        const std::string seed = std::format("random_moves_per_turn_{}", exl::util::GetRandomU64());
        auto rng = RngManager::NewRandomGenerator(std::span(seed));
        std::array<s16, 4> moves;
        rng.RandMoves(std::span(moves));
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2C8 + 0x1C * 0) = moves[0];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2D4 + 0x1C * 0) = moves[0];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2C8 + 0x1C * 1) = moves[1];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2D4 + 0x1C * 1) = moves[1];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2C8 + 0x1C * 2) = moves[2];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2D4 + 0x1C * 2) = moves[2];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2C8 + 0x1C * 3) = moves[3];
        *reinterpret_cast<s16*>(ctx->X[0] + 0x2D4 + 0x1C * 3) = moves[3];
    }
};

HOOK_DEFINE_INLINE(RandomMovesFirstTurn) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        const std::string seed = std::format("random_moves_per_turn_{}", exl::util::GetRandomU64());
        auto rng = RngManager::NewRandomGenerator(std::span(seed));
        std::array<s16, 4> moves;
        rng.RandMoves(std::span(moves));
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2C8 + 0x1C * 0) = moves[0];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2D4 + 0x1C * 0) = moves[0];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2C8 + 0x1C * 1) = moves[1];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2D4 + 0x1C * 1) = moves[1];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2C8 + 0x1C * 2) = moves[2];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2D4 + 0x1C * 2) = moves[2];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2C8 + 0x1C * 3) = moves[3];
        *reinterpret_cast<s16*>(ctx->X[20] + 0x2D4 + 0x1C * 3) = moves[3];
    }
};

void install_random_moves_per_turn_patch() {
    RandomMovesPerTurn::InstallAtOffset(0x7e2dd0);
    RandomMovesFirstTurn::InstallAtOffset(0x7e09d8);
}