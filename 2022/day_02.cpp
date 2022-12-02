#include <iostream>
#include <assert.h>

#include <aoc.h>

enum RPS {
	ROCK,
	PAPER,
	SCISSORS,
};

enum GameResult {
	LOSE,
	DRAW,
	WIN,
};

int result_table[3] = { 0, 3, 6 };

int rock_paper_scissors(RPS enemy, RPS you) {
	static int score_look_up[3][3];
	score_look_up[ROCK][ROCK] = DRAW;
	score_look_up[ROCK][PAPER] = WIN;
	score_look_up[ROCK][SCISSORS] = LOSE;

	score_look_up[PAPER][ROCK] = LOSE;
	score_look_up[PAPER][PAPER] = DRAW;
	score_look_up[PAPER][SCISSORS] = WIN;

	score_look_up[SCISSORS][ROCK] = WIN;
	score_look_up[SCISSORS][PAPER] = LOSE;
	score_look_up[SCISSORS][SCISSORS] = DRAW;

	int score_idx = score_look_up[enemy][you];
	int score = result_table[score_idx];
	return score + (you + 1);
}

RPS predict_outcome(RPS enemey, GameResult result) {
	static RPS look_up[3][3];
	look_up[ROCK][WIN] = PAPER;
	look_up[ROCK][DRAW] = ROCK;
	look_up[ROCK][LOSE] = SCISSORS;

	look_up[PAPER][WIN] = SCISSORS;
	look_up[PAPER][DRAW] = PAPER;
	look_up[PAPER][LOSE] = ROCK;

	look_up[SCISSORS][WIN] = ROCK;
	look_up[SCISSORS][DRAW] = SCISSORS;
	look_up[SCISSORS][LOSE] = PAPER;

	return look_up[enemey][result];
}

int main() {
	std::string content = read_entire_file("input.txt");
	uint64_t final_score_01 = 0;
	uint64_t final_score_02 = 0;
	for (const auto lines : split(content, "\n")) {
		std::vector<std::string_view> moves = split(lines, " ");
		assert(moves.size() == 2);

		std::string_view elves_play = moves[0];
		std::string_view you_play = moves[1];

		RPS elves_type;
		RPS player_type;
		GameResult player_result;

		if (elves_play == "A") { elves_type = ROCK; }
		else if (elves_play == "B") { elves_type = PAPER; }
		else { assert(elves_play == "C"); elves_type = SCISSORS; }

		if (you_play == "X") { player_type = ROCK; player_result = LOSE; }
		else if (you_play == "Y") { player_type = PAPER; player_result = DRAW; }
		else { assert(you_play == "Z"); player_type = SCISSORS; player_result = WIN; }

		int result_01 = rock_paper_scissors(elves_type, player_type);
		final_score_01 += result_01;

		RPS should_play = predict_outcome(elves_type, player_result);
		int result_02 = rock_paper_scissors(elves_type, should_play);
		final_score_02 += result_02;
	}

	std::cout << "Part 01: " << final_score_01 << "\n";
	std::cout << "Part 02: " << final_score_02 << "\n";
}