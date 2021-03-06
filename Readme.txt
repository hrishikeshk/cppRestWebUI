
Introduction:
------------

The software contained with this package implements,
1.A Rest API for serving typical requests around stock markets.
2.A Web UI that consumes the API.

The source includes a visual studio project (within 'cppRestWebUI')along with the C++ files. However, additional installations
as well as configurations are still required to use the project.

Environment, Install instructions, Dependencies:
-----------------------------------------------

This package is developed using these third party libraries,
1. Microsoft Visual Studio Community 2015
2. Wt 3.3.6 (https://www.webtoolkit.eu/wt). 
		Download and install the Windows binary listed and as per the target being 32/64 bit OS.
		Note the paths to 'Wt' include', 'bin' and 'lib' directories for later use.
3. Casablanca C++ REST SDK 2.8.0 (https://github.com/Microsoft/cpprestsdk). 
		Use the Tools->NuGet package manager->Manage packages for Solution within Visual Studio 2015 to first search 
		for 'casablanca C++ REST SDK' and then find version 2.8.0 by casablancacore and install it for the project.
4. Sqlite 3.14 (www.sqlite.org)
		This should comprise of amalgamation of exactly two files - one header (.h) and one source (.cpp).
		These source files should be added to the project as source to be build along with the application itself.
		Secondly, also download command line utilities/.exe for running database scripts.

Database initializer script:
---------------------------

This software uses a relational databse 'Sqlite'. 
As a pre-requisite,
1. The script in a file named 'DB_Script.sql' should be run.
2. The name of the database is 'Market.db'.
3. To run script, use the following command, the executable for sqlite is assumed sqlite3.exe.

		type DBScript.sql | sqlite3.exe Market.db

4. Copy the generated file 'Market.db' to the Visual studio project directory (alongside '.vcxproj' file so that the application
	finds it in the current directory).

Configuration and preparing source:
----------------------------------

Use the Visual studio project file 'cppRestWebUI.vcxproj' within the folder 'cppRestWebUI' to view project source.
While using Visual Studio 2015 for building, the following steps are required,
at least the following stepswere needed above the code to make the build and execution work out.
1. It uses a console application project.
2. The project is set as Startup Project.
3. In project configuration -
	a.  - Debugging - use this as command line arguments,
			--http-address=0.0.0.0 --http-port=8080 --deploy-path=/ --docroot=.
	b.  - Environement - add the path to the 'bin' directory of 'Wt' installation.
	c.  - C/C++ - general - add the 'Wt' include directory to the additional include paths.
	d.  - C/C++ - Preprocessor - 'WIN32' should be present in the preprocessor definitions.
	e.  - C/C++ - advanced - add '4996' to - disable specific warnings (required for Wt).
	f.  - Linker - general - In the additional library directories, add path to the 'Wt' lib folder
	g.  - Linker - Input - Additional Dependencies - add these library objects for linking with Wt if the project is a debug build,
					wtd.lib, wthttpd.lib, wtdbod.lib, wtdbosqlite3d.lib, libboost_unit_test_framework-vc140-mt-gd-1_59.lib
					If the project is not a 'Debug' build then add these,
					wt.lib, wthttp.lib, wtdbo.lib, wtdbosqlite3.lib, 
					libboost_unit_test_framework-vc140-mt-gd-1_59.lib

4.  Use NuGet to fetch and install casablanca C++ REST SDK 2.8.0.
5.  The build nevertheless shows several warnings from 'Wt' but without any errors and ready.
6. Build and run source. It should start up a server listening to port specified (8080) and accessible 
    using a browser on the loopback address (127.0.0.1).

Assumptions and missing requirements:
------------------------------------

The REST API has been added as separate entry points to the Wt application and Casablanca functionality is 
being used for specific processing functions. So the REST service runs in same HTTP endpoint as the Web UI but 
with a distinct resource.

Missing functionality that is desirable,
1. Forgot password, unregister trading
2. Add/List/De-list stocks. Initial prices and stock quantities
3. Stock price settings.
4. CAPTHCA during new user registration.
5. Logging in a file of the operations, particularly errors/exceptions.

Issues:
------

Certain statements in the requirements specification needed clarification and usually some client contact is available
to sort them out before development and design. I could make some assumptions to proceed but in doing the design, I spent more time
than expected.


* Above all, it was an interesting project for me to do and am eager for feedback.
#########################################
