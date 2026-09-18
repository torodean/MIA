/**
 * @file Playlist.hpp
 * @author Antonius Torode
 * @date 09/18/2026
 * @brief An ordered list of audio tracks with a cursor and shuffle memory.
 */
#pragma once

#include <cstddef>
#include <vector>

// Used for the track metadata type.
#include "FileUtils.hpp"

namespace audio
{
	/**
	 * @brief An ordered list of audio tracks with a cursor and shuffle memory.
	 *
	 * This class owns the playlist state which the AudioPlayer plays through:
	 * the ordered tracks, the index of the track which is playing (or is next),
	 * and the shuffle memory of previously played track indexes. This class
	 * only manages that state; it never plays anything itself.
	 *
	 * @note This class is not thread-safe. The caller is responsible for
	 *       synchronizing access while a playlist is being played.
	 */
	class Playlist
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Playlist() = default;

		/**
		 * @brief Constructs this playlist from a list of tracks.
		 *
		 * The index is set to the first entry and the shuffle memory is
		 * empty, since the saved indexes would no longer be meaningful.
		 *
		 * @param tracks The tracks to store. These are stored as-is, so the
		 *        caller is responsible for only passing supported types.
		 */
		explicit Playlist(const std::vector<files::FileMetaData>& tracks);

		/**
		 * @brief Replaces the tracks in this playlist.
		 *
		 * The index is reset to the first entry and the shuffle memory is
		 * cleared, since the saved indexes would no longer be meaningful.
		 *
		 * @param tracks The tracks to store. These are stored as-is, so the
		 *        caller is responsible for only passing supported types.
		 * @return true if the playlist is non-empty after being set, false
		 *         otherwise.
		 */
		bool setList(const std::vector<files::FileMetaData>& tracks);

		/**
		 * @brief Adds a single track to the end of the playlist.
		 */
		void add(const files::FileMetaData& track);

		/**
		 * @brief Removes all tracks, resets the index, and clears the
		 *        shuffle memory.
		 */
		void clear();

		/**
		 * @brief Provides the track at the current index.
		 *
		 * @return The track at the current index, or a default (Unknown
		 *         type) track when the playlist is empty or the index is
		 *         past the end.
		 */
		files::FileMetaData current() const;

		/**
		 * @brief Moves the index to the next track.
		 *
		 * The current index is recorded in the shuffle memory before it is
		 * moved. With shuffle enabled the new index is picked at random
		 * rather than the next sequential one, and the random pick is never
		 * the index being moved away from. The index wraps around to the
		 * first track when looping is enabled. Calling this on an empty
		 * playlist leaves the state unchanged.
		 *
		 * @param shuffle Whether or not to pick the new index at random.
		 * @param loop Whether or not the index wraps around after the last track.
		 * @return true if the index was moved to another track, false when
		 *         it was left unchanged (empty playlist, or the next index
		 *         would be past the end with looping disabled).
		 */
		bool advance(bool shuffle, bool loop);

		/**
		 * @brief Moves the index to the most recently recorded entry in the
		 *        shuffle memory and consumes that entry.
		 *
		 * The index being moved away from is not recorded anywhere.
		 *
		 * @return true if the index was moved to a previously played track,
		 *         false when the shuffle memory is empty (the index is left
		 *         unchanged).
		 */
		bool back();

	private:
		/**
		 * @brief Computes the index of the next track.
		 *
		 * @param shuffle Whether or not to pick the new index at random.
		 * @param loop Whether or not the index wraps around after the last track.
		 * @return The index of the next track, or a value past the end of the
		 *         playlist when the playlist is finished (no wrap).
		 */
		size_t selectNextIndex(bool shuffle, bool loop) const;

		/**
		 * @brief Records the current index in the shuffle memory.
		 */
		void pushToHistory();

		/// The tracks in this playlist, in play order.
		std::vector<files::FileMetaData> tracks;
		/// The index of the track which is playing or is next.
		size_t index{0};
		/// The previously played track indexes, most recent last.
		std::vector<size_t> history;
	}; // class Playlist
} // namespace audio
