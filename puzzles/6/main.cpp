#include <fstream>
#include <iostream>
#include <deque>
#include <unordered_set>

size_t uniqueSize(const std::deque<char> &d);

size_t uniqueSize(const std::deque<char> &d)
{
  // this seems terribly inefficient
  std::unordered_set<char> s;
  for (const char &c : d) {
    s.insert(c);
  }
  return s.size();
}

int main()
{
  std::ifstream in("puzzles/6/input.txt");

  const size_t PACKET_MARKER_LENGTH{4};
  const size_t MESSAGE_MARKER_LENGTH{14};
  bool foundStartMarkerForPacket = false;
  int packetStart = 0;
  std::deque<char> packetSearchSeq;

  bool foundStartMarkerForMessage = false;
  int messageStart = 0;
  std::deque<char> messageSearchSeq;

  char c = in.get();

  while (in) {
    if (foundStartMarkerForPacket == false) {
      packetStart++;

      packetSearchSeq.push_back(c);
      while (packetSearchSeq.size() > PACKET_MARKER_LENGTH) {
        packetSearchSeq.pop_front();
      }

      if (uniqueSize(packetSearchSeq) == PACKET_MARKER_LENGTH) {
        foundStartMarkerForPacket = true;
      }
    }

    if (foundStartMarkerForMessage == false) {
      messageStart++;

      messageSearchSeq.push_back(c);
      while (messageSearchSeq.size() > MESSAGE_MARKER_LENGTH) {
        messageSearchSeq.pop_front();
      }

      if (uniqueSize(messageSearchSeq) == MESSAGE_MARKER_LENGTH) {
        foundStartMarkerForMessage = true;
      }
    }

    if (foundStartMarkerForPacket && foundStartMarkerForMessage) {
      break;
    }

    c = in.get();
  }

  std::cout << "packetStart: " << packetStart << std::endl;
  std::cout << "messageStart: " << messageStart << std::endl;

  return 0;
}