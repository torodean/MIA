/**
 * @file Playlist.cpp
 * @author Antonius Torode
 * @date 09/18/2026
 * @brief Implements the methods from the associated header file.
 */

// The associated header file.
#include "Playlist.hpp"

// Used for the playlist shuffle randomization.
#include "MathUtils.hpp"

namespace audio
{
    Playlist::Playlist(const std::vector<files::FileMetaData>& tracks)
    {
        setList(tracks);
    }


    bool Playlist::setList(const std::vector<files::FileMetaData>& tracks)
    {
        this->tracks = tracks;
        index = 0;
        history.clear();
        return !this->tracks.empty();
    }


    void Playlist::add(const files::FileMetaData& track)
    {
        tracks.push_back(track);
    }


    void Playlist::clear()
    {
        tracks.clear();
        index = 0;
        history.clear();
    }


    files::FileMetaData Playlist::current() const
    {
        // An empty playlist or an index past the end gives an empty (Unknown) track.
        if (tracks.empty() || index >= tracks.size())
            return files::FileMetaData{};
        return tracks[index];
    }


    bool Playlist::advance(bool shuffle, bool loop)
    {
        size_t nextIndex = selectNextIndex(shuffle, loop);

        // An empty playlist or a next index past the end leaves the state unchanged.
        if (nextIndex >= tracks.size())
            return false;

        // Record the current index so back() can restore it.
        pushToHistory();
        index = nextIndex;
        return true;
    }


    bool Playlist::back()
    {
        // There is nothing to restore when the memory is empty.
        if (history.empty())
            return false;

        // Restore the index to the most recently recorded entry.
        index = history.back();
        history.pop_back();
        return true;
    }


    const std::vector<files::FileMetaData>& Playlist::getTracks() const
    { 
        return tracks;
    }


    size_t Playlist::getCurrentIndex() const
    { 
        return index;
    }


    size_t Playlist::selectNextIndex(bool shuffle, bool loop) const
    {
        // The playlist is empty, so there is nothing to select.
        if (tracks.empty())
            return 0;

        if (shuffle && tracks.size() > 1)
        {
            /*
             * Pick a random index which is not the current index so shuffle never
             * picks the same track twice in a row. The retry handles the small
             * chance of the random pick landing on the current index.
             */
            for (int i = 0; i < 5; ++i)
            {
                /*
                 * The default arguments seed from std::random_device and request
                 * time seeding, so each pick differs.
                 */
                size_t pick = static_cast<size_t>(
                    math::randomInt(0, static_cast<int>(tracks.size() - 1)));
                if (pick != index)
                    return pick;
            }
            return (index + 1) % tracks.size();
        }

        // The last track is current, so wrap around or signal the playlist is finished.
        if (index + 1 >= tracks.size())
        {
            if (!loop)
                return tracks.size(); // Past the end signals the playlist is finished.
            return 0; // Wrap around to the first track.
        }

        return index + 1;
    }


    void Playlist::pushToHistory()
    {
        // An index past the end does not point at a track, so there is nothing to record.
        if (index >= tracks.size())
            return;
        history.push_back(index);
    }
} // namespace audio
