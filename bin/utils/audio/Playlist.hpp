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
	 * @brief A playlist of audio tracks with a current-track cursor and a
	 *        memory of previously played tracks.
	 *
	 * This class owns the playlist state which the AudioPlayer plays through:
	 * the ordered tracks, the index of the track which is playing (or is next),
	 * and the shuffle memory used to go back through previously played tracks.
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
		 * The index is reset to the first entry and the shuffle memory is
		 * cleared, since the saved indexes would no longer be meaningful.
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
		 * @return The current track, or a default (Unknown type) track when
		 *         the playlist is empty or the index is past the end.
		 */
		files::FileMetaData current() const;

		/**
		 * @brief Advances to the next track.
		 *
		 * The track being left is recorded in the shuffle memory. With
		 * shuffle enabled this picks a random track which is not the current
		 * track; otherwise this is the next sequential track. The playlist
		 * wraps back to the first track when looping is enabled.
		 * Calling this on an empty playlist does nothing.
		 *
		 * @param shuffle Whether or not to pick the next track at random.
		 * @param loop Whether or not to wrap back around after the last track.
		 * @return true if the index moved to another track, false when the
		 *         playlist is finished or empty.
		 */
		bool advance(bool shuffle, bool loop);

		/**
		 * @brief Goes back to the most recently played track.
		 *
		 * The current track is not recorded anywhere; the shuffle memory
		 * entry which is consumed is the track being returned to.
		 *
		 * @return true if the index moved back to a previously played track,
		 *         false when the shuffle memory is empty.
		 */
		bool back();

	private:
		/**
		 * @brief Selects the index of the next track to play.
		 *
		 * @param shuffle Whether or not to pick the next track at random.
		 * @param loop Whether or not to wrap back around after the last track.
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
