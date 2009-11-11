﻿using System;
using System.Collections.Generic;
using System.Text;

namespace Org.Irduco.UpdateManager
{
    public class BlockedGamesManager
    {
        private string updateURL;
        private string localPath;
        private List<GameInfo> blockedGames;
        private Dictionary<string, GameInfo> blockedGamesDict;
        private BlockedGamesHelper helper;
        BlockedGameType blockageType;

        public enum  BlockedGameType
        {
            None = 0,
            FromInternetThanLocal = 1,
            Internet = 2,
            Local = 3,
        }

        public string UpdateURL
        {
            get
            {
                return updateURL;
            }
        }

        public BlockedGamesManager(string updateURL, string path, BlockedGameType blockageType)
        {
            this.updateURL = updateURL;
            this.localPath = path;
            this.blockageType = blockageType;
        }

        public BlockedGameType GetBlockedGames()
        {
            if (this.blockageType == BlockedGameType.FromInternetThanLocal)
            {
                try
                {
                    GetBlockedGames(false);
                    return BlockedGameType.Internet;
                }
                catch (Exception ex)
                {
                    GetBlockedGames(true);
                    return BlockedGameType.Local;
                }
            }
            else if (this.blockageType == BlockedGameType.Internet)
            {
                GetBlockedGames(false);
                return BlockedGameType.Internet;
            }
            else if (this.blockageType == BlockedGameType.Local)
            {
                GetBlockedGames(true);
                return BlockedGameType.Local;
            }
            else
            {
                return BlockedGameType.None;
            }


        }

        private void GetBlockedGames(bool useLocal)
        {
            helper = new BlockedGamesHelper(updateURL);
            if (useLocal)
            {
                blockedGames = helper.ReadBlockedGameList(localPath);
            }
            else
            {
                blockedGames = helper.GetBlockedGameList();
            }
            blockedGamesDict = new Dictionary<string, GameInfo> ();
            foreach(GameInfo game in blockedGames) 
            {
                blockedGamesDict.Add(game.DiscId, game);
            }
        }

        public void UpdateLocalBlockedGames()
        {
            helper.SaveBlockedGameList(localPath, blockedGames);
        }

        public GameInfo IsGameBlocked(string discId)
        {
            if (blockedGamesDict.ContainsKey(discId))
            {
                return blockedGamesDict[discId];
            }
            else
            {
                return null;
            }
        }
    }
}
