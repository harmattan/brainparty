Brain Party Port
================

Brain Party is a puzzle-solving, brain-stretching game that comes with
36 minigames designed to push your brain to its limits, testing memory,
logic, mathematics, reactions and more.

This game was written for Apple's iPhone, and you can buy it in the
App Store for $0.99 - in fact, doing so helps support me a little!

Because I used non-free music in the iPhone release, this version uses
new music - see the CREDITS file for more information.


Compiling from source
=====================

I haven't used any fancy tools, so all you need to do to build Brain Party
is run "make" and give it a few minutes. If you have a multi-core CPU, try
"make -j4" to get a speed boost.

You will need the development libraries for SDL, SDL Mixer, SDL TTF, SDL GFX,
SDL Image and OpenGL. Obviously you will also need a C++ compiler like G++.


Version history
===============

 * *1.0.0 - January 10th 2015*
   - Cleaned up ports tree.

 * *v0.1 - March 5th 2010*
   - First release.


Reading the source
==================

Brain Party was originally written for Linux in C#, using Mono and SDL.NET. 
It was then ported to Windows Mobile 5.0, which also uses C#, but has only
the .NET Compact Framework. From there, it was ported to the Xbox 360 (and
that port is still available through the Microsoft Indie Games store), which
also uses C# and the .NET Compact Framework, plus the XNA Framework.

From there, the game was ported to the iPhone, which meant rewriting it in
Objective C++ and OpenGL. Some of the code was taken from the X360 version
as it had been polished; other code was taken from the WinMo code, because
it was already built for touchscreens.

The Brain Party you have now descends from the iPhone build. All the code
that was Objective C++ is now plain C++.

The reason I'm telling you all this is because, if you look at the code, 
you'll see it's packed with ideosyncrasies. That's because the ports I've 
done were usually done in less than a week, so rather than rewriting
everything from scratch I've usually just implemented shims that accomplish
the same thing. In other places, I've sorely missed functionality that was
available elsewhere, so I've re-implemented it myself.

So, the advice is this: look at the code if you want to. But, whatever you
do, don't learn from it - it's the dirtiest C++ you'll ever see.

This will be rectified over time. Possibly.


Contributing
============

If you'd like to contribute to Brain Party, great! Get in touch with me at
paul.hudson@gmail.com and we'll talk. Code, graphics, sound effects, music,
etc are all welcome. If you spot any bugs, drop me a line with some sort of
helpful error message - ie, what I have to do to reproduce the error - and
I'll get on the case.
