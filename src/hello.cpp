#include <stdio.h>
#include <unistd.h>
#include <framework/mlt.h>

int main(int argc, char *argv[])
{
  // Initialise the factory
  if (mlt_factory_init("/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt"))
  {
    // Create a profile
    mlt_profile profile = mlt_profile_init(NULL);

    // Create the default consumer
    mlt_consumer hello = mlt_factory_consumer(profile, NULL, NULL);

    // Create via the default producer
    mlt_producer world = mlt_factory_producer(profile, NULL, argv[1]);

    // Connect the producer to the consumer
    mlt_consumer_connect(hello, mlt_producer_service(world));

    // Start the consumer
    mlt_consumer_start(hello);

    // Wait for the consumer to terminate
    while (!mlt_consumer_is_stopped(hello)) {
      sleep(1);
    }

    // Close the consumer
    mlt_consumer_close(hello);

    // Close the producer
    mlt_producer_close(world);

    // Close the profile
    mlt_profile_close(profile);

    // Close the factory
    mlt_factory_close();
  }
  else
  {
    // Report an error during initialisation
    fprintf(stderr, "Unable to locate factory modules\n");
  }

  // End of program
  return 0;
}