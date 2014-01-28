InLimbo
=======

In Limbo Uni CG Projekt



How to add Libraries in Visual Studio 2013
	Move compiled library to folder ...\madlibs\<library>
	In VS Project > Project Properties (Artefact Engine Properties)
	On C/C++ > General at Additional Include Libraries add $(SolutionDir)\madlibs\<library>\include
	On Linker > General at Additional Library Directories Dependencies add $(SolutionDir)\madlibs\<library>\lib
	On Linker > Input at Additional Dependencies add <library file.lib>
	
