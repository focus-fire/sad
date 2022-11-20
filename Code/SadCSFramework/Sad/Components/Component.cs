using System;

namespace Sad
{
    public abstract class Component
    {
        public Guid EntityGuid { get; internal set; }
    }
}
