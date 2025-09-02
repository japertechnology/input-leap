/*
    InputLeap -- mouse and keyboard sharing utility
    Copyright (C) InputLeap contributors

    This package is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    found in the file LICENSE that should have accompanied this file.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace inputleap {

/**
 * @brief Supported fingerprint algorithms.
 *
 * The enumeration maps the identifiers used within configuration files
 * to the algorithms implemented in the codebase.  SHA1 is retained for
 * backwards compatibility but should not be used for new installations.
 */
enum FingerprintType {
    INVALID, /**< Unrecognised or unsupported algorithm. */
    SHA1,    /**< SHA-1 fingerprint. Deprecated. */
    SHA256,  /**< SHA-256 fingerprint. */
};

/**
 * @brief Stores the raw fingerprint and associated metadata.
 *
 * The @c algorithm string identifies the hashing algorithm used to
 * generate @c data.  The byte vector holds the raw fingerprint digest.
 */
struct FingerprintData {
    /** Algorithm identifier such as "sha256". */
    std::string algorithm;
    /** Raw digest produced by the algorithm. */
    std::vector<std::uint8_t> data;

    /**
     * @brief Determine whether the fingerprint contains valid data.
     *
     * A fingerprint is considered valid when an algorithm name is
     * present.  This does not validate the contents of @c data.
     */
    bool valid() const { return !algorithm.empty(); }

    /**
     * @brief Compare two fingerprints for equality.
     *
     * Equality is defined by both the algorithm identifier and the raw
     * digest.  This facilitates use of FingerprintData in standard
     * containers and algorithms.
     */
    bool operator==(const FingerprintData& other) const;
};

/**
 * @brief Convert a FingerprintType to its textual representation.
 * @param type Enumeration value to convert.
 * @return String literal corresponding to @p type or "invalid" if the
 *         type is unknown.
 */
const char* fingerprint_type_to_string(FingerprintType type);

/**
 * @brief Parse a textual fingerprint algorithm identifier.
 * @param type Case-sensitive algorithm name.
 * @return Corresponding enumeration value or FingerprintType::INVALID
 *         when the name is not recognised.
 */
FingerprintType fingerprint_type_from_string(const std::string& type);

} // namespace inputleap
