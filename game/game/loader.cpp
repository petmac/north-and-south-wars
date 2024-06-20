#include "loader.h"

bool load(Resource resource) {
  switch (resource) {
  case Resource::smallFont:
    return false;

  case Resource::palette:
    return false;
  }

  return false;
}
