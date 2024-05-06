#include "Guesser.h"
#include <string>
#include <algorithm>

using std::string;
using std::min;
using std::max;

const unsigned int MAX_SECRET_LENGTH = 32;
const unsigned int MAX_GUESSES = 3;


unsigned int Guesser::distance(string guess) {
    unsigned int guessLength = guess.length();
    unsigned int secretLength = m_secret.length();
    unsigned int lengthDifference = abs(static_cast<int>(secretLength) - static_cast<int>(guessLength));
    
    unsigned int minLength = min(secretLength, guessLength);
    unsigned int differences = 0;

    for (unsigned int i = 0; i < minLength; ++i) {
        if (m_secret[i] != guess[i]) {
            ++differences;
        }
    }

    return differences + lengthDifference;
}


Guesser::Guesser(string secret) {
    if (secret.length() > MAX_SECRET_LENGTH) {
        secret = secret.substr(0, MAX_SECRET_LENGTH);
    }
    m_secret = secret;
    m_guessesRemaining = MAX_GUESSES;
    m_locked = false;
}


bool Guesser::match(string guess) {
    if (m_locked || m_guessesRemaining == 0) {
        return false;
    }

    unsigned int dist = distance(guess);

    if (dist == 0) {
        m_guessesRemaining = MAX_GUESSES;
        return true;
    } else {
        if (dist > 2) {
            m_locked = true;
            m_guessesRemaining = 0;
        } else {
            if (--m_guessesRemaining == 0) {
                m_locked = true;
            }
        }
        return false;
    }
}

/**
 * Returns the number of guesses remaining.
 */
unsigned int Guesser::remaining() {
    return m_guessesRemaining;
}


